using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ManagedClient
{
    public static class EnumWindowsPInvoke
    {
        private delegate bool EnumWindowsProc(IntPtr hwnd, int lParam);

        [DllImport("user32.Dll", SetLastError = true)]
        private static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, int lParam);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern int GetWindowTextLength(IntPtr hWnd);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);

        [DllImport("user32.dll", SetLastError = true)]
        private static extern bool IsWindowVisible(IntPtr hWnd);

        public static IReadOnlyList<WindowInfo2> EnumWindows()
        {
            List<WindowInfo2> result = new();
            EnumWindows((IntPtr hwnd, int lParam) =>
            {
                int length = GetWindowTextLength(hwnd);
                var total = length + 1;
                var sb = new StringBuilder(total);
                if(GetWindowText(hwnd, sb, total) == 0 && length != 0)
                {
                    //Console.WriteLine($"error {length} 0x{hwnd.ToString("x")}");
                    return true;
                }

                string title = sb.ToString();
                bool isVisible = IsWindowVisible(hwnd);

                result.Add(new WindowInfo2(hwnd.ToInt32(), title, isVisible));
                return true;
            }, 10);

            return result;
        }

    }

    public record WindowInfo2(int HWnd, string Title, bool IsVisible);

}
