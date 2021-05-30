using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Microsoft.Windows.Sdk;

namespace ShellAction.ShellActions
{
    public class DesktopWallpaperActions : Shell, IShell
    {
        private static readonly Guid CLSID_DesktopWallpaper = new Guid("{C2CF3110-460E-4fc1-B9D0-8A1C0C9CC4BD}");

        private static readonly string Identifier = "-desktopwallpaper";
        private Dictionary<string, Action<IDesktopWallpaper, string[]>> actions;

        public DesktopWallpaperActions()
        {
            actions = new Dictionary<string, Action<IDesktopWallpaper, string[]>>()
            {
                { "-next", (instance, args) => SlideshowNext(instance, args)},
                { "-previous", (instance, args) => SlideshowPrevious(instance, args)},
                { "-time", (instance, args) => SlideshowTime(instance, args)},
            };
        }

        public string ShowUsage()
        {
            StringBuilder sb = new();
            sb.AppendLine($"{Identifier}");
            sb.AppendLine($"Parameters:");
            foreach (var action in actions)
                sb.AppendLine($"  {action.Key}");
            return sb.ToString();
        }

        public bool CanHandle(string[] args)
        {
            if (args.Where(a => a.StartsWith(Identifier)).Any()) return true;
            return false;
        }

        public bool ExecuteAction(string[] args)
        {
            bool res = false;
            var instance = GetInstance();
            foreach(var action in GetActions(args, actions))
            {
                action(instance, args);
                res = true;
            }

            return res;
        }

        private static IDesktopWallpaper GetInstance()
        {
            Type typeDesktopWallpaper = Type.GetTypeFromCLSID(CLSID_DesktopWallpaper);
            return (IDesktopWallpaper)Activator.CreateInstance(typeDesktopWallpaper);
        }

        private void SlideshowNext(IDesktopWallpaper instance, string[] args)
        {
            instance.AdvanceSlideshow(null, DESKTOP_SLIDESHOW_DIRECTION.DSD_FORWARD);
        }

        private void SlideshowPrevious(IDesktopWallpaper instance, string[] args)
        {
            instance.AdvanceSlideshow(null, DESKTOP_SLIDESHOW_DIRECTION.DSD_BACKWARD);
        }

        private void SlideshowTime(IDesktopWallpaper instance, string[] args)
        {
            uint milliseconds = 10000;
            var par = "-time:";
            var timeArg = args.FirstOrDefault(a => a.StartsWith(par));
            if (timeArg != null && timeArg.Length > par.Length)
            {
                uint.TryParse(timeArg.Substring(par.Length), out milliseconds);
            }

            instance.SetSlideshowOptions(
                DESKTOP_SLIDESHOW_OPTIONS.DSO_SHUFFLEIMAGES,
                milliseconds);
        }

    }
}
