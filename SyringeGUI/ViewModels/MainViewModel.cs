using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Input;
using static Syringe.Models.Process;

namespace Syringe.ViewModels
{
    public class MainViewModel : INotifyPropertyChanged
    {
        private List<Models.Process> cachedProcesses = new List<Models.Process>();

        public string DllPath { get; set; }


        private bool show32BitProcesses = true;
        public bool Show32BitProcesses {
            get
            {
                return show32BitProcesses;
            }
            set
            {
                show32BitProcesses = value;
                UpdateProcessesLayout(Show64BitProcesses, Show32BitProcesses, NameSearched);
            }
        }

        private bool show64BitProcesses = true;
        public bool Show64BitProcesses
        {
            get
            {
                return show64BitProcesses;
            }
            set
            {
                show64BitProcesses = value;
                UpdateProcessesLayout(Show64BitProcesses, Show32BitProcesses, NameSearched);
            }
        }


        private string nameSearched = string.Empty;
        public string NameSearched
        {
            get
            {
                return nameSearched;
            }
            set
            {
                nameSearched = value;
                UpdateProcessesLayout(Show64BitProcesses, Show32BitProcesses, NameSearched);
            }
        }


        private List<Models.Process> processes;
        public List<Models.Process> Processes
        {
            get
            {
                return processes;
            }
            set
            {
                processes = value;
                NotifyPropertyChanged();
            }
        }


        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Notifies the View when a property in the ViewModel has changed
        /// </summary>
        /// <param name="propertyName">Name of the property that has changed</param>
        private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }


        public MainViewModel() => InitializeProcesses();


        /// <summary>
        /// Initializes the processes list, caching all current running processes
        /// </summary>
        public void InitializeProcesses()
        {
            cachedProcesses?.Clear();

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

                // Omit invalid processes
                if (processInfo.Architecture != Architectures.undetermined)
                    cachedProcesses.Add(processInfo);
            }

            UpdateProcessesLayout(show64BitProcesses, Show32BitProcesses, NameSearched);
        }


        /// <summary>
        /// Updates the Processes property, which is the one used for the layout
        /// </summary>
        /// <param name="show64">True if the 64-bit processes will be shown</param>
        /// <param name="show32">True if the 32-bit processes will be shown</param>
        /// <param name="nameContains">If given, will show only the processes that their name contain this parameter</param>
        private void UpdateProcessesLayout(bool show64, bool show32, string nameContains = "")
        {
            List<Models.Process> aux = new List<Models.Process>(cachedProcesses);

            if (!show64)
                aux = aux.Where(x => x.Architecture != Architectures.x64).ToList();

            if (!show32)
                aux = aux.Where(x => x.Architecture != Architectures.x86).ToList();

            if (nameContains != "")
            {
                nameContains = nameContains.ToUpper();
                aux = aux.Where(x => x.Name.ToUpper().Contains(nameContains)).ToList();
            }

            // Update only if the processes count has changed
            if (Processes == null || Processes.Count != aux.Count)
                Processes = aux.OrderBy(x => x.Name)
                                .ToList();
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
                    result = !(IsWow64Process(process.Handle, out bool retValue) && retValue) ? Architectures.x64 : Architectures.x86;
            }

            // Access to the process was denied
            catch (Win32Exception exception) { }
            
            return result;
        }
    }
}
