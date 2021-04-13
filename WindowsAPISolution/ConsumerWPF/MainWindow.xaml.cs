using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using Windows.Devices.Enumeration;
using Windows.Media.AppRecording;
using Windows.Media.Capture;
using Windows.Media.MediaProperties;
using Windows.Storage;
using Windows.Web.Syndication;

namespace ConsumerWPF
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private async void Window_Loaded(object sender, RoutedEventArgs e)
        {
            if (OperatingSystem.IsWindowsVersionAtLeast(10))
            {
            }
            if (OperatingSystem.IsWindows())
            {
            }

            var locator = new Windows.Devices.Geolocation.Geolocator();
            var location = await locator.GetGeopositionAsync();
            var position = location.Coordinate.Point.Position;
            var latlong = $"lat:{position.Latitude:0.##}, long:{position.Longitude:0.##}";
            //var result = MessageBox.Show(latlong); Debug.WriteLine($"Position is {position}");
            txtLocation.Text = $"Position is {latlong}";
        }

        private async void TakePicture_Click(object sender, RoutedEventArgs e)
        {
            // Initialize the webcam
            MediaCapture captureManager = new MediaCapture();
            await captureManager.InitializeAsync();

            ImageEncodingProperties imgFormat = ImageEncodingProperties.CreateJpeg();
            // create storage file in local app storage
            StorageFile file =
                        await KnownFolders.CameraRoll.CreateFileAsync("TestPhoto.jpg",
                                           CreationCollisionOption.GenerateUniqueName);

            // take photo
            await captureManager.CapturePhotoToStorageFileAsync(imgFormat, file);

            // Get photo as a BitmapImage
            BitmapImage bmpImage = new BitmapImage(new Uri(file.Path));
            // CameraImage is an <Image> control defined in XAML
            CameraImage.Source = bmpImage;

        }

        //private async void TakePicture_Click(object sender, RoutedEventArgs e)
        //{
        //    // Initialize the webcam
        //    var settings = new MediaCaptureInitializationSettings();
        //    MediaCapture captureManager = new MediaCapture();
        //    if (!await InitializeAvailableCamera(captureManager, settings)) return;

        //    ImageEncodingProperties imgFormat = ImageEncodingProperties.CreateJpeg();
        //    // create storage file in local app storage
        //    StorageFile file =
        //                await KnownFolders.CameraRoll.CreateFileAsync("TestPhoto.jpg",
        //                                   CreationCollisionOption.GenerateUniqueName);

        //    // take photo
        //    await captureManager.CapturePhotoToStorageFileAsync(imgFormat, file);

        //    // Get photo as a BitmapImage
        //    BitmapImage bmpImage = new BitmapImage(new Uri(file.Path));
        //    // CameraImage is an <Image> control defined in XAML
        //    CameraImage.Source = bmpImage;

        //}

        private async Task<bool> InitializeAvailableCamera(MediaCapture mediaCapture, MediaCaptureInitializationSettings settings)
        {
            var devices = await DeviceInformation.FindAllAsync(DeviceClass.VideoCapture);
            foreach (var device in devices)
            {
                settings.VideoDeviceId = device.Id;
                try
                {
                    await mediaCapture.InitializeAsync(settings);
                    return true;
                }
                catch (Exception)
                {
                    continue;
                }
            }

            return false;
        }

        private async void TakeScreenshot_Click(object sender, RoutedEventArgs e)
        {
            StorageFile file = await KnownFolders.CameraRoll.CreateFileAsync("TestScreenshot.mp4",
                               CreationCollisionOption.GenerateUniqueName);
            var folder = await file.GetParentAsync();
            var path = folder.Path;
            //var folder = await StorageFolder.GetFolderFromPathAsync(".");
            //var folder = (await StorageLibrary.GetLibraryAsync(KnownLibraryId.Pictures)).SaveFolder;

            AppRecordingManager manager = AppRecordingManager.GetDefault();
            var status = manager.GetStatus();
            if (status.CanRecord || status.CanRecordTimeSpan)
            {
                var myVideo = await StorageLibrary.GetLibraryAsync(KnownLibraryId.Videos);
                StorageFolder projectFolder = await myVideo.SaveFolder.CreateFolderAsync("DataFolder",
                    CreationCollisionOption.OpenIfExists);

                var audio = await projectFolder.CreateFileAsync("abc.mp4", CreationCollisionOption.GenerateUniqueName);

                var result = await manager.StartRecordingToFileAsync(audio).AsTask<AppRecordingResult>();




                //var result = await manager.StartRecordingToFileAsync(file);
                //var result = await manager.SaveScreenshotToFilesAsync(
                //    folder,
                //    "screenshot",
                //    AppRecordingSaveScreenshotOption.HdrContentVisible,
                //    manager.SupportedScreenshotMediaEncodingSubtypes);

                Debug.WriteLine(result.Succeeded);
                //if (result.Succeeded)
                //{
                //    foreach (var item in result.SavedScreenshotInfos)
                //    {
                //        Debug.WriteLine(item.File.DisplayName);
                //    }
                //}
                //else
                //{
                //    Debug.WriteLine(result.ExtendedError.Message);
                //}
            }
        }

        private async void TakeRssTitles_Click(object sender, RoutedEventArgs e)
        {
            var dispatcher = Application.Current.Dispatcher;
            var client = new SyndicationClient();
            var uri = new Uri("http://www.iamraf.net/rss");

            var feed = await client.RetrieveFeedAsync(uri);

            foreach (var item in feed.Items)
            {
                listBox1.Items.Add(item.Title.Text);
            }

        }


    }
}
