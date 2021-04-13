using System;
using Microsoft.Windows.Sdk;

namespace InteropWin32Console
{
    class Program
    {
        static void Main(string[] args)
        {
            int hr = Constants.S_OK;
            PInvoke.Sleep(100);
        }
    }
}
