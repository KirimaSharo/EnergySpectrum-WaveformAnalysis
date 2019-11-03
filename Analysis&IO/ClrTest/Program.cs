using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using AnalysisClr;
using IOClr;

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
            A.WaveThreshold = 150;
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
                wav[i] = cnt[i];
            }
            w.SampleValue = wav;
            //e.Count = cnt;
            //e.Analysis_De_LM_EM(A);
            //for (int i = 0; i < e.AnalysisResultLength; i++)
            //{
            //    Console.WriteLine(e.AnalysisResult[i].Peak);
            //}

            bool bl=w.Analysis(A);
            Console.WriteLine(bl);
            for (int i = 0; i < w.AnalysisResultLength; i++)
            {
                Console.WriteLine(w.AnalysisResult[i].TimeofMax);
            }
            Structedfile sf = new Structedfile();
            sf.ReadFile("", "1234");
            Structedfile sf2 = new Structedfile();
            sf2 = sf;
            sf2.WriteFile("", "ABCDEFG");

            //Byte[] Frame = { 0x55, 0xAA,
            //    0x14,0x01,0xFD,0x00,0x01,0x00,0x04,
            //    19,10,31,15,30,00,
            //    0xEB,0x90,0xA1,0x0A,0x01,
            //    0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            //    0xC1,0xA1,
            //    0xEE
            //};
            //Rawdatafile raw = new Rawdatafile();
            //unsafe
            //{
            //    Byte* FrameESP = stackalloc Byte[280];
            //    for (int j = 0; j < 32; j++)
            //    {
            //        for (int i = 0; i < 280; i++)
            //        {

            //            FrameESP[i] = Frame[i];
            //            FrameESP[i] = (Byte)i;
            //        }
            //        raw.WriteFrame((IntPtr)FrameESP);
            //    }
            //}
            //raw.Run = 1000;

            //Structedfile strf = new Structedfile();
            //strf = raw.AnalysisFrameCache();
            
            
            Console.ReadLine();
        }
    }
}
