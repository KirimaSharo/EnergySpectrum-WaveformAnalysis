﻿using System;
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
            EnergySP e = new EnergySP();
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
            e.Count = cnt;
            AnalysisParam A = new AnalysisParam();
            A.ESPCalibrationX = "x";
            A.ESPCalibrationY = "x";
            A.WaveCalibrationX = "x";
            A.WaveCalibrationY = "x";
            A.WaveThreshold = 0;
            A.nmax = 10;
            A.ESPMaxResolution = 0.2;
            e.Analysis_De_LM_EM(A);
            for(int i=0;i<e.AnalysisResultLength;i++)
            {
                Console.WriteLine(e.AnalysisResult[i].Peak);
            }
            Console.ReadLine();
        }
    }
}
