using System;
using System.Collections.Generic;
using System.Windows.Forms;

using System.Diagnostics;

namespace table_gen
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new frm_main());

            ////  强制结束所有excel进程
            //Process[] ps = Process.GetProcesses();
            //foreach (Process item in ps)
            //{
            //    if (item.ProcessName == "EXCEL")
            //    {
            //        item.Kill();
            //    }
            //}
        }
    }
}