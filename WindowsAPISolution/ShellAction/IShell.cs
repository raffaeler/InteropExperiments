using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShellAction
{
    public interface IShell
    {
        bool CanHandle(string[] args);

        bool ExecuteAction(string[] args);

        string ShowUsage();
    }
}
