using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using BenchmarkDotNet.Attributes;

using WinNativeUtilities;

namespace ManagedClient
{
    [MemoryDiagnoser]
    [DisassemblyDiagnoser(printSource: true)]
    public class EnumWindowsBenchmark
    {
        private Windowing _wnd;
        public EnumWindowsBenchmark()
        {
            _wnd = new();
        }

        [Benchmark]
        public void WinRTInterop()
        {
            var windows = _wnd.EnumWindows();
        }

        [Benchmark]
        public void PInvokeInterop()
        {
            var windows = EnumWindowsPInvoke.EnumWindows();
        }

    }
}
