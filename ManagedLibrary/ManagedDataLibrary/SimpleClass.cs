using System;
using System.Runtime.InteropServices;

namespace ManagedDataLibrary
{
    public class SimpleClass
    {
        public static void Print(IntPtr ptrMessage)
        {
            Console.Clear();
            Console.WriteLine();
            Console.WriteLine();

            var message = Marshal.PtrToStringUTF8(ptrMessage);
            Console.WriteLine(message);
        }
    }

}
