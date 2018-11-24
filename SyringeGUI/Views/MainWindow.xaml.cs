using Microsoft.Win32;
using Syringe.Bindings;
using Syringe.Models;
using Syringe.ViewModels;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Syringe.Views
{
    /// <summary>
    /// Lógica de interacción para MainWindow.xaml
    /// </summary>
    public partial class MainWindow : BaseWindow
    {
        private MainViewModel ViewModel => MainGrid.DataContext as MainViewModel;


        public MainWindow()
        {            
            InitializeComponent();
        }


        private void SearchDll_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                DllPath.Text = SelectDll();
            }

            catch (Exception ex)
            {
                Error(ex);
            }
        }


        private void UpdateProcesses_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                ViewModel.InitializeProcesses();
            }

            catch (Exception ex)
            {
                Error(ex);
            }
        }


        private void InjectDll_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                Process selectedProcess = ProcessesList.SelectedItem as Process;
                IntPtr injectedDllAndress = Injector.InjectDll(DllPath.Text, selectedProcess.Name + ".exe");

                if (injectedDllAndress != default(IntPtr))
                {
                    string msg = string.Format("\"{0}\" injected correctly at address {1}", DllPath.Text, injectedDllAndress);

                    Log(msg);
                    MessageBox.Show(msg, "Dll injected", MessageBoxButton.OK);
                }
            }

            catch (Exception ex)
            {
                Error(ex);
            }
        }


        /// <summary>
        /// Shows a OpenFileDialog to select a dll
        /// </summary>
        /// <returns>Path of the selected DLL or null otherwise</returns>
        private string SelectDll()
        {
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.DefaultExt = "*.dll";
            dialog.Filter = "Dynamic Link Library (*.dll)|*.dll";

            if (dialog.ShowDialog() == true)
                return dialog.FileName;

            return null;
        }
    }
}
