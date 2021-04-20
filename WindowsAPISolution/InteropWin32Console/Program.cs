using System;
using System.Collections.Generic;
using System.Diagnostics;

using Microsoft.Windows.Sdk;

namespace InteropWin32Console
{
    class Program
    {
        static void Main(string[] args)
        {
            int hr = Constants.S_OK;
            PInvoke.Sleep(100);

            var utilities = new Utilities();
            var visibileWindows = utilities.GetVisibleWindows();
            foreach (var title in visibileWindows)
            {
                Console.WriteLine(title);
            }
        }





    }
}
