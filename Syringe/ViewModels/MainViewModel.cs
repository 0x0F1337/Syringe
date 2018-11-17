using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using static Syringe.Models.Process;

namespace Syringe.ViewModels
{
    public class MainViewModel
    {
        public bool Show32BitProcesses { get; set; } = true;
        public bool Show64BitProcesses { get; set; } = true;

        public List<Models.Process> AllProcesses { get; set; } = new List<Models.Process>();

        // TODO:
        // public ICommand Show32BitCommand => new Action();


        public MainViewModel(bool show32, bool show64)
        {
            Show32BitProcesses = show32;
            Show64BitProcesses = show64;

            SetupProcesses(Show32BitProcesses, Show64BitProcesses);
        }


        private void SetupProcesses(bool show32, bool show64)
        {
            AllProcesses.Clear();
            Process[] allProcesses = Process.GetProcesses();

            for (int i = 0; i < allProcesses.Length; i++)
            {
                Process process = allProcesses[i];

                Models.Process processInfo = new Models.Process()
                {
                    PID = process.Id,
                    Name = process.ProcessName,
                    Architecture = IsWow64Process(process)
                };

                if ((processInfo.Architecture == Architectures.x64 && show64) ||
                    (processInfo.Architecture == Architectures.x86 && show32))
                    AllProcesses.Add(processInfo);
            }

            AllProcesses = AllProcesses.OrderBy(x => x.Name).ToList();
        }


        [DllImport("kernel32.dll", SetLastError = true, CallingConvention = CallingConvention.Winapi)]
        [return: MarshalAs(UnmanagedType.Bool)]
        internal static extern bool IsWow64Process([In] IntPtr process, [Out] out bool wow64Process);

        /// <summary>
        /// Checks wether a process is 64-bit or 32-bit
        /// </summary>
        /// <see cref="https://stackoverflow.com/a/1953411"/>
        /// <param name="process">Process to be checked</param>
        /// <returns>The architecture of the process</returns>
        private Architectures IsWow64Process(Process process)
        {
            Architectures result = Architectures.undetermined;

            try
            {
                if ((Environment.OSVersion.Version.Major > 5) ||
                   ((Environment.OSVersion.Version.Major == 5) &&
                   (Environment.OSVersion.Version.Minor >= 1)))
                    result = (IsWow64Process(process.Handle, out bool retValue) && retValue) ? Architectures.x64 : Architectures.x86;
            }

            // Access to the process was denied
            catch (Win32Exception exception) { }
            
            return result;
        }
    }
}
