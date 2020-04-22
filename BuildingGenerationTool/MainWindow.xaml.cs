using System;
using System.Collections.Generic;
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
using System.Diagnostics;
using System.IO;
using Microsoft.Win32;
using System.Text.RegularExpressions;
using System.Threading;

namespace BuildingGenerationTool
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string rootDirectory = Directory.GetParent(Environment.CurrentDirectory).Parent.Parent.FullName;
        private string directXSaveDirectory = "\\DirectXTool\\DirectXTool\\Data";
        private string condaEnvrionment = "";
        private const string lineDetectionScript = "\\LineDetection\\LineDetection.py";
        private const string imageClassifierScript = "\\ImageClassifier\\Classifier.py";
        private string baseImageDir = "";

        public MainWindow()
        {
            InitializeComponent();
        }

        public void RunBothPythonScriptsOnImage()
        {
            if (condaEnvrionment != "")
            {
                var process = new Process
                {
                    StartInfo = new ProcessStartInfo
                    {
                        FileName = "cmd.exe",
                        RedirectStandardInput = true,
                        UseShellExecute = false,
                        RedirectStandardOutput = true,
                        WorkingDirectory = rootDirectory + "\\ImageClassifier"
                    }
                };

                process.Start();

                using (var sw = process.StandardInput)
                {
                    if (sw.BaseStream.CanWrite)
                    {
                        sw.WriteLine("C:\\Users\\ben11\\Miniconda3\\Scripts\\activate.bat");
                        sw.WriteLine("conda activate tensorflow");
                        sw.WriteLine("python " + rootDirectory + imageClassifierScript + " " + EstimatedSymbolHeight.Text + " " + EstimatedSymbolWidth.Text + " " + baseImageDir + " " + rootDirectory + directXSaveDirectory);
                    }
                }

                // read multiple output lines
                while (!process.StandardOutput.EndOfStream)
                {
                    var line = process.StandardOutput.ReadLine();
                    Console.WriteLine(line);
                }
                process.Close();

                RunJustLineDetectionPythonScript(baseImageDir, true);
                //Finished
            }
        }

        /// <summary>
        /// Will run our python line detection code. The paramater has to be specified because we may be using the sliced image from imageclassification instead of the raw base image
        /// </summary>
        /// <param name="imageLocation"></param>
        public void RunJustLineDetectionPythonScript(string imageLocation, bool haveSymbolsBeenLoaded)
        {
            if (condaEnvrionment != "")
            {
                var process = new Process
                {
                    StartInfo = new ProcessStartInfo
                    {
                        FileName = "cmd.exe",
                        RedirectStandardInput = true,
                        UseShellExecute = false,
                        RedirectStandardOutput = true,
                        WorkingDirectory = rootDirectory + "\\LineDetection"
                    }
                };

                process.Start();

                // Pass multiple commands to cmd.exe
                using (var sw = process.StandardInput)
                {
                    if (sw.BaseStream.CanWrite)
                    {
                        sw.WriteLine("C:\\Users\\ben11\\Miniconda3\\Scripts\\activate.bat");
                        sw.WriteLine("conda activate tensorflow");

                        //minLineLength = int(sys.argv[1])
                        //maxLineGap = int(sys.argv[2])
                        //detectionThreshold = int(sys.argv[3])
                        //_minDistToMerge = int(sys.argv[4])
                        //_minMergeAngle = int(sys.argv[5])
                        //imageLocation = sys.argv[6]
                        //saveLocation = sys.argv[7]
                        sw.WriteLine("python " + rootDirectory + lineDetectionScript + " " + MinLineGap.Text + " " + MaxLineGap.Text + " " + DetectionThreshold.Text + " " 
                            + MinMergeDistance.Text + " " + MinMergeAngle.Text +  " " + imageLocation + " " +
                            rootDirectory + directXSaveDirectory + " " + haveSymbolsBeenLoaded);
                        sw.WriteLine("conda deactivate tensorflow");
                    }
                }

                // read multiple output lines
                while (!process.StandardOutput.EndOfStream)
                {
                    var line = process.StandardOutput.ReadLine();
                    Console.WriteLine(line);
                }

                process.Close();

                //Finished
                SetImage(GeneratedImage, System.IO.Path.Combine(Directory.GetParent(System.IO.Directory.GetCurrentDirectory()).Parent.Parent.FullName, "LineDetection\\MergedLines.jpg"));
            }
        }

        private void UpdateFileLocation(object sender, RoutedEventArgs e)
        {
            using (var dialog = new System.Windows.Forms.FolderBrowserDialog())
            {
                System.Windows.Forms.DialogResult result = dialog.ShowDialog();
                MinicondaDirectory.Content = dialog.SelectedPath;
                condaEnvrionment = dialog.SelectedPath;
            }
        }

        private void BrowseButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.InitialDirectory = "c:\\";
            dlg.Filter = "Image files (*.jpg) |*.jpg; |All Files (*.*)|*.*";
            dlg.RestoreDirectory = true;
           
            if (dlg.ShowDialog() ?? false)
            {
                baseImageDir = dlg.FileName;
                SetImage(BaseImage, dlg.FileName);   
            }
        }

        private void DetectFeatures(object sender, RoutedEventArgs e)
        {
            bool isCheckingSymbols = DetectSymbolsCheckbox.IsChecked.HasValue ? DetectSymbolsCheckbox.IsChecked.Value : false;

            if (isCheckingSymbols)
            {
                RunBothPythonScriptsOnImage();
            }
            else
            {   
                RunJustLineDetectionPythonScript(baseImageDir, false);
            }
        }

        private void SetImage(Image targetElement, string fileName)
        {
            string selectedFileName = fileName;
            ImageDirectory.Content = selectedFileName;
            BitmapImage bitmap = new BitmapImage();
            bitmap.BeginInit();
            bitmap.UriSource = new Uri(selectedFileName);
            bitmap.EndInit();
            targetElement.Source = bitmap;
        }

        private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
        {
            Regex regex = new Regex("[^0-9]+");

            if (regex.IsMatch(e.Text))
            {
                e.Handled = regex.IsMatch(e.Text);
            }
        }
    }
}
