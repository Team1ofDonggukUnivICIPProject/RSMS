using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using MetroFramework;
using MetroFramework.Forms;

using OpenCvSharp;
using System.Threading;

/*
 * Prototype#1
 * 
 * Author   :   Doryeong Park
 * Date     :   7. 12. 2016
 * Desc     :   This code is for test of simple motion detection algorithm with C#
 */
namespace Prototype
{
    public partial class PrototypeForm : MetroForm
    {

        private Thread frameRunner;
        private VideoCapture camera;
        private Mat currentFrame;
        private Mat previousFrame;
        private Mat resultFrame;
        private Mat posterizedFrame;

        private const int GCCOUNT = 30;

        private int motionIndex;
      
        public PrototypeForm()
        {
            InitializeComponent();
        }

        private void OnLoad(object sender, EventArgs e)
        {
            currentFrame = new Mat();
            posterizedFrame = new Mat();
            previousFrame = new Mat();

            frameRunner = new Thread(new ThreadStart(delegate () { RunFrame(); }));
            frameRunner.Start();
        }


        private void RunFrame()
        {
            CheckForIllegalCrossThreadCalls = false;

            var gcCounter = 0;

            //Camera from webcam
            camera = new VideoCapture(0);

            while (true)
            {
                //Grab 1 frame
                camera.Read(currentFrame);
                posterizedFrame = new Mat();
                picb_original.ImageIpl = currentFrame;

                Cv2.CvtColor(currentFrame, posterizedFrame, ColorConversionCodes.BGR2GRAY);

                //8 Level Posterizing
                EightLevelPosterizing();

                //Subtract Images
                resultFrame = posterizedFrame - previousFrame;

                motionIndex = 0;
                var resultFrameIndexer = resultFrame.GetGenericIndexer<byte>();

                //Remove noises simply
                for (int i = 0; i < resultFrame.Height; ++i)
                {
                    for (int j = 0; j < resultFrame.Width; ++j)
                    {
                        var currentPixel = resultFrameIndexer[i, j];
                        if (currentPixel <= 96)
                            resultFrameIndexer[i, j] = 0;
                        else
                            ++motionIndex;
                    }
                }

                //This value can be sensitivity
                if (motionIndex >= 100)
                {
                    lbl_motionnotifier.Text = "Motion Detected";
                }
                else
                {
                    lbl_motionnotifier.Text = "";
                }

                picb_result.ImageIpl = resultFrame;
                previousFrame = posterizedFrame;

                //Compel garbage collector collect memory of image
                if(GCCOUNT == gcCounter++)
                {
                    gcCounter = 0;
                    GC.Collect();
                }
                
                var ch = Cv2.WaitKey(1);
                if (ch == 27)
                    break;
            }
        }
        
        /* Mapping values lowest value of each 8 ranges */
        private void EightLevelPosterizing()
        {
            var frameIndexer = posterizedFrame.GetGenericIndexer<byte>();
            for (int i = 0; i < posterizedFrame.Height; ++i)
                for (int j = 0; j < posterizedFrame.Width; ++j)
                {
                    var currentPixel = frameIndexer[i, j];
                    if (0 <= currentPixel && currentPixel <= 32)
                        frameIndexer[i, j] = 0;
                    else if (32 < currentPixel && currentPixel <= 64)
                        frameIndexer[i, j] = 32;
                    else if (64 < currentPixel && currentPixel <= 96)
                        frameIndexer[i, j] = 64;
                    else if (96 < currentPixel && currentPixel <= 128)
                        frameIndexer[i, j] = 96;
                    else if (128 < currentPixel && currentPixel <= 160)
                        frameIndexer[i, j] = 128;
                    else if (160 < currentPixel && currentPixel <= 192)
                        frameIndexer[i, j] = 160;
                    else if (192 < currentPixel && currentPixel <= 224)
                        frameIndexer[i, j] = 192;
                    else
                        frameIndexer[i, j] = 224;
                }
        }

        private void OnDestroy(object sender, FormClosedEventArgs e)
        {
            camera.Dispose();
        }
    }
}
