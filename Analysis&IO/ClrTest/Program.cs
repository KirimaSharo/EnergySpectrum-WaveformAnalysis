using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using AnalysisClr;

namespace ClrTest
{
    class Program
    {
        static void Main(string[] args)
        {
            AnalysisParam A = new AnalysisParam();
            A.ESPCalibrationX = "x";
            A.ESPCalibrationY = "x";
            A.WaveCalibrationX = "x";
            A.WaveCalibrationY = "x";
            A.WaveThreshold = 0;
            A.nmax = 10;
            A.ESPMaxResolution = 0.2;

            //EnergySP e = new EnergySP();
            WaveForm w = new WaveForm();
            String filepath = "a.dat";
            int a;
            FileStream fs = new FileStream(filepath, FileMode.Open, FileAccess.ReadWrite);
            BinaryReader br = new BinaryReader(fs);
            a = br.ReadInt32();
            int[] cnt = new int[a];
            for (int i = 0; i < a; i++)
            {
                cnt[i] = br.ReadInt32();
            }
            fs.Close();
            double[] wav = new double[a];
            for (int i = 0; i < a; i++)
            {
                wav[i] = -1;
                if (i % 2 == 0)
                    wav[i] = 1;
            }
            w.SampleValue = wav;
            //e.Count = cnt;
            //e.Analysis_De_LM_EM(A);
            //for (int i = 0; i < e.AnalysisResultLength; i++)
            //{
            //    Console.WriteLine(e.AnalysisResult[i].Peak);
            //}

            bool bl=w.FFT(A);
            Console.WriteLine(bl);
            for (int i = 0; i < a; i++)
            {
                Console.WriteLine(w.FFTResult[i]);
            }
                Console.ReadLine();
        }
    }
}
