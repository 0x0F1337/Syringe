#pragma once

#include "Common.h"
#include "WinProc.h"
#include "Utils.h"


#define NULL_TERMINATOR_LENGTH 1


extern "C" {
	/// <summary>
	/// Injects a DLL in a process given the DLL path and the target process name.
	/// Uses CreateRemoteThread for the injection, so keep an eye on antimalwares!
	/// </summary>
	/// <param name="dllPath">The path for the DLL</param>
	/// <param name="processName">The process name where the DLL will be injected</param>
	/// <returns>Memory address where the DLL has been injected</returns>
	DllExport LPVOID InjectDll(LPCSTR dllPath, LPSTR processName);


	/// <summary>
	/// Injects a DLL in a process given the DLL path and the target process name.
	/// Avoids the use of the function CreateRemoteThread using APCs
	///
	/// Don't use this function for CLR injection, as it doesn't garanty that the dll is executed.
	/// Needs the remote process to enter an alertable state.
	/// </summary>
	/// <see cref="http://blogs.microsoft.co.il/pavely/2017/03/14/injecting-a-dll-without-a-remote-thread/"/>
	/// <param name="dllPath">The path for the DLL</param>
	/// <param name="processName">The process name where the DLL will be injected</param>
	/// <returns>Memory address where the DLL has been injected</returns>
	DllExport LPVOID InjectDllApc(LPCSTR dllPath, LPSTR processName);


	/// <summary>
	/// Starts the Common Language Runtime (aka .NET platform) in a remote process.
	/// It is needed that a DLL is injected in the remote process before this method can work,
	/// and that the DLL exports a function that loads the CLR.
	/// </summary>
	/// <param name="processName">Name of the target process</param>
	/// <param name="dllPath">Full path of the dll that was injected in the process</param>
	/// <param name="entryPointFunction">Function located in the injected DLL that starts the CLR</param>
	DllExport void StartRemoteCLR(LPSTR processName, LPCSTR dllPath, LPCSTR entryPointFunction);
}