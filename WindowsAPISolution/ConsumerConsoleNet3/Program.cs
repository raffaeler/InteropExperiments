using System;
using System.Threading.Tasks;

namespace ConsumerConsoleNet3
{
    class Program
    {
        static void Main(string[] args)
        {
            new Program().Start();
            Console.ReadKey();
        }

        private async void Start()
        {
            Console.WriteLine("Hello World!");
            await Task.Delay(0);

            var _locator = new Windows.Devices.Geolocation.Geolocator();
            var location = await _locator.GetGeopositionAsync();
            var position = location.Coordinate.Point.Position;
            Console.WriteLine($"Lat:{position.Latitude}, Long:{position.Longitude}, Alt:{position.Altitude}");
        }
    }
}
