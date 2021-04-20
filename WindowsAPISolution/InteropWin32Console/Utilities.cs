using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Microsoft.Windows.Sdk;

namespace InteropWin32Console
{
    class Utilities
    {
        public IList<string> GetVisibleWindows()
        {
            List<(int hr, HWND hWnd, string title)> windows = new();
            PInvoke.EnumWindows(WndCallback, (LPARAM)1234);

            var titles = windows
                .Where(w => PInvoke.IsWindowVisible(w.hWnd) && !string.IsNullOrEmpty(w.title))
                .Select(w => w.title)
                .ToList();

            unsafe bool WndCallback(HWND hWnd, LPARAM lParam)
            {
                Debug.Assert(lParam.Value == 1234);

                var len = PInvoke.GetWindowTextLength(hWnd) + 1;// null terminated
                var stackMemory = stackalloc char[len]; 
                PWSTR title = new PWSTR(stackMemory);
                var result = PInvoke.GetWindowText(hWnd, title, len);

                windows.Add((result, hWnd, result > 0 ? new string(title) : null));
                return true;
            }

            return titles;
        }
    }
}
