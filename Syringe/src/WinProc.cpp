#include "WinProc.h"


DWORD GetProcessId(LPSTR processName) {
	DWORD processId;

	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (strcmp(entry.szExeFile, processName) == 0)
			{
				processId = entry.th32ProcessID;
				break;
			}
		}
	}

	CloseHandle(snapshot);

	return processId;
}


HANDLE GetProcessHandle(LPSTR processName) {
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (strcmp(entry.szExeFile, processName) == 0)
			{
				hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
				break;
			}
		}
	}

	CloseHandle(snapshot);

	return hProcess;
}


Process GetProcessInfo(LPSTR processName) {
	Process pInfo;

	pInfo.pid = GetProcessId(processName);
	pInfo.hProcess = GetProcessHandle(processName);
	pInfo.mainModule = GetMainModuleInfo(processName, processName);
	pInfo.allThreads = GetProcessThreadsIds(processName);

	return pInfo;
}


MODULEINFO GetMainModuleInfo(LPSTR moduleName, LPSTR processName) {
	HANDLE hProcess = GetProcessHandle(processName);

	MODULEINFO result;
	HMODULE hMods[1024];

	char currentModuleName[1024];
	DWORD cbNeeded;

	// Enumerates all the process modules and iterates over them
	if (EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL)) {
		size_t modulesLength = (cbNeeded / sizeof(HMODULE));

		for (int i = 0; i < modulesLength; i++) {
			HMODULE hCurrentModule = hMods[i];

			GetModuleBaseNameA(hProcess, hCurrentModule, currentModuleName, sizeof(currentModuleName));

			if (strcmp(currentModuleName, moduleName) == 0) {
				GetModuleInformation(hProcess, hCurrentModule, &result, sizeof(result));
				break;
			}
		}
	}

	CloseHandle(hProcess);

	return result;
}


std::vector<DWORD> GetProcessThreadsIds(LPSTR processName) {
	std::vector<DWORD> result;

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPTHREAD, NULL);
	DWORD pid = GetProcessId(processName);

	THREADENTRY32 entry{ sizeof(entry) };

	if (::Thread32First(hSnapshot, &entry)) {
		do {
			if (entry.th32OwnerProcessID == pid)
				result.push_back(entry.th32ThreadID);

		} while (::Thread32Next(hSnapshot, &entry));
	}

	return result;
}