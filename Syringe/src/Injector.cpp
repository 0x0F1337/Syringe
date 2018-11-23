#include "Injector.h"


LPVOID InjectDll(LPCSTR dllPath, LPSTR processName) {
	Process processInfo = GetProcessInfo(processName);
	int dllPathLength = strlen(dllPath) + NULL_TERMINATOR_LENGTH;

	// Allocate memory for the dllpath in the target process
	LPVOID pDllPath = VirtualAllocEx(processInfo.hProcess, 0, dllPathLength, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// Write the path to the address of the memory we just allocated in the target process
	WriteProcessMemory(processInfo.hProcess, pDllPath, (LPCVOID)dllPath, dllPathLength, 0);

	// Create a Remote Thread in the target process which
	// calls LoadLibraryA as our dllpath as an argument -> program loads our dll
	HANDLE hLoadThread = CreateRemoteThread(processInfo.hProcess, 0, 0,
		(LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"),
			"LoadLibraryA"), pDllPath, 0, 0);

	// Wait for the execution of our loader thread to finish
	WaitForSingleObject(hLoadThread, INFINITE);

	// Free the memory allocated for our dll path and close its handle
	VirtualFreeEx(processInfo.hProcess, pDllPath, dllPathLength, MEM_RELEASE);
	CloseHandle(hLoadThread);

	return pDllPath;
}


LPVOID InjectDllApc(LPCSTR dllPath, LPSTR processName) {
	int dllPathLength = strlen(dllPath) + NULL_TERMINATOR_LENGTH;
	HANDLE hProcess = GetProcessHandle(processName);
	std::vector<DWORD> allThreadsIds = GetProcessThreadsIds(processName);

	// Allocate memory for the DLL path
	LPVOID pDllPath = VirtualAllocEx(hProcess, 0, dllPathLength, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	// Write the path to the address of the memory we just allocated in the target process
	WriteProcessMemory(hProcess, pDllPath, (LPCVOID)dllPath, dllPathLength, 0);

	// Iterate over every thread and queue an APC
	for (DWORD threadId : allThreadsIds) {
		HANDLE hThread = OpenThread(THREAD_SET_CONTEXT, FALSE, threadId);

		QueueUserAPC((PAPCFUNC)::GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), hThread, (ULONG_PTR)pDllPath);
	}

	// Free the memory allocated for our dll path and close its handle
	VirtualFreeEx(hProcess, pDllPath, dllPathLength, MEM_RELEASE);
	CloseHandle(hProcess);

	return pDllPath;
}


void StartRemoteCLR(LPSTR processName, LPCSTR dllPath, LPCSTR entryPointFunction) {
	std::string dllName = GetFileNameFromPath(std::string(dllPath));

	HANDLE hProcess = GetProcessHandle(processName);
	MODULEINFO mod = GetMainModuleInfo(&dllName[0], processName);

	// The offset of the entry function in the DLL in reference to the base address, will be the same in any process,
	// so we calculate it locally
	HMODULE tempDllLoad = LoadLibrary(dllPath);
	void* tempDllFuncAddr = GetProcAddress(tempDllLoad, entryPointFunction);

	DWORD_PTR offset = (DWORD_PTR)tempDllFuncAddr - (DWORD_PTR)tempDllLoad;

	HANDLE hThread = CreateRemoteThread(hProcess,
										NULL,
										NULL,
										(LPTHREAD_START_ROUTINE)((DWORD_PTR)mod.lpBaseOfDll + offset),
										NULL,
										NULL,
										NULL);

	WaitForSingleObject(hThread, INFINITE);

	// Release allocated handles
	CloseHandle(hThread);
	CloseHandle(hProcess);
}