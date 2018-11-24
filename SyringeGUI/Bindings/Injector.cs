using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Syringe.Bindings
{
    public class Injector
    {
        private const string DLL_NAME = "Syringe.dll";


        [DllImport(DLL_NAME, CallingConvention = CallingConvention.Cdecl)]
        /// <summary>
        /// Injects a DLL in a process given the DLL path and the target process name.
        /// Uses CreateRemoteThread for the injection, so keep an eye on antimalwares!
        /// </summary>
        /// <param name="dllPath">The path for the DLL</param>
        /// <param name="processName">The process name where the DLL will be injected</param>
        /// <returns>Memory address where the DLL has been injected</returns>
        public static extern IntPtr InjectDll(string dllPath, string processName);

    }
}
