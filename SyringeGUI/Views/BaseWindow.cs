using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Syringe.Views
{
    public class BaseWindow : Window
    {
        // TODO: Implement proper logging
        protected void Log(string log)
        {

        }


        // TODO: Implement proper logging
        protected void Error(Exception ex)
        {
            Debug.WriteLine(ex.ToString());
        }
    }
}
