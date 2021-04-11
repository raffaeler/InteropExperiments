using System;

using WinNativeUtilities;

namespace ManagedClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            Security helper = new Security();
            helper.MyProperty = 10;
            Console.WriteLine(helper.MyProperty);

            var result = helper.CreateLowProcess(@"c:\windows\system32\notepad.exe", string.Empty);
            Console.WriteLine($"{result}");
        }
    }
}
