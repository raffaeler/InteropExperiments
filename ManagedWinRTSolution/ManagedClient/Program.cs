using System;
using System.Diagnostics;
using System.Linq;

using BenchmarkDotNet.Reports;
using BenchmarkDotNet.Running;

using Microsoft.Diagnostics.Runtime.ICorDebug;

using WinNativeUtilities;

namespace ManagedClient
{
    class Program
    {
        static int Main(string[] args)
        {
            //return DoBenchmark();

            //TestSecurity();

            Windowing wnd = new();
            var windows = wnd.EnumWindows();
            var count = windows.Count;
            var filtered1 = windows
                .Where(w => w.Title.Length > 0)
                .OrderBy(w => w.Title)
                .ToList();

            //foreach (var wininfo in filtered1)
            //{
            //    Console.WriteLine(wininfo.Title);
            //}


            var windows2 = EnumWindowsPInvoke.EnumWindows();
            var filtered2 = windows2
                .Where(w => w.Title.Length > 0)
                .OrderBy(w => w.Title)
                .ToList();

            var filtered1b = filtered1
                .Select(w => new WindowInfo2(w.HWnd, w.Title, w.IsVisible))
                .ToList();

            //var diff = filtered1b.Except(filtered2).Union(
            //    filtered2.Except(filtered1b))
            //    .ToList();


            foreach (var wininfo in filtered1b.Except(filtered2))
            {
                Console.WriteLine($"Item in winrt and not in pinvoke: {wininfo.Title}");
            }

            foreach (var wininfo in filtered2.Except(filtered1b))
            {
                Console.WriteLine($"Item in pinvoke but not in winrt: {wininfo.Title}");
            }


            return 0;
        }


        static void TestSecurity()
        {
            Security helper = new Security();
            helper.MyProperty = 10;
            Console.WriteLine(helper.MyProperty);

            var result = helper.CreateLowProcess(@"c:\windows\system32\notepad.exe", string.Empty);
            Console.WriteLine($"{result}");
        }


        static int DoBenchmark()
        {
            // due to a bug in the current release of BenchmarkDotNet
            // we have to run some measurement using a raw stopwatch
            // https://github.com/dotnet/BenchmarkDotNet/issues/1616

            //Summary summary;
            //summary = BenchmarkRunner.Run<EnumWindowsBenchmark>();

            var count = 400;

            Windowing wnd = new();
            wnd.EnumWindows();
            EnumWindowsPInvoke.EnumWindows();

            Stopwatch _sw = new();

            _sw.Start();
            for (var i = 0; i < count; i++)
            {
                wnd.EnumWindows();
            }

            var winrt = _sw.Elapsed;
            Console.WriteLine(winrt);
            Console.WriteLine();

            _sw.Reset();
            _sw.Start();
            for (var i = 0; i < count; i++)
            {
                EnumWindowsPInvoke.EnumWindows();
            }

            var pinvoke = _sw.Elapsed;

            Console.WriteLine($"Winrt: {winrt.TotalMilliseconds}  -  PInvoke: {pinvoke.TotalMilliseconds}");

            return 0;
        }

    }
}
