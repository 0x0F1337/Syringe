using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Syringe.Models
{
    public class Process
    {
        public enum Architectures
        {
            undetermined,
            x86,
            x64
        }

        public string Name { get; set; }
        public Architectures Architecture { get; set; }
        public int PID { get; set; }
    }
}
