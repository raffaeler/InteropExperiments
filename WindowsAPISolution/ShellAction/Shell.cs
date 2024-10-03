using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShellAction;

public abstract class Shell
{
    public IEnumerable<Action<T, string[]>> GetActions<T>(string[] args, Dictionary<string, Action<T, string[]>> actions)
    {
        Action<T, string[]> action;
        foreach (var arg in args)
        {
            var index = arg.IndexOf(':');
            var trimmed = arg;
            if (index != -1)
            {
                trimmed = arg.Substring(0, index);
            }

            if (actions.TryGetValue(trimmed, out action)) yield return action;
        }
    }

}
