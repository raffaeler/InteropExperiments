using System;
using System.Numerics;

using Microsoft.Windows.Sdk;

using ShellAction.ShellActions;

namespace ShellAction;

class Program
{
    static int Main(string[] args)
    {
        var actions = new IShell[]
        {
            new DesktopWallpaperActions(),
        };

        if (args.Length == 0) return ShowUsage(actions);

        bool res = false;
        foreach (var action in actions)
        {
            if (action.CanHandle(args)) res = res | action.ExecuteAction(args);
        }

        return res ? 0 : -1;
    }

    private static int ShowUsage(IShell[] actions)
    {
        Console.WriteLine("ShellAction, Copyright (c) Raffaele Rialdi 2021, @raffaeler");
        Console.WriteLine("Utility to execute some action over the supported objects");
        Console.WriteLine("Command line accepts the following format: -option[:parameter]");
        Console.WriteLine("Available actions:");
        foreach (var action in actions)
        {
            Console.WriteLine(action.ShowUsage());
        }

        return -1;
    }
}
