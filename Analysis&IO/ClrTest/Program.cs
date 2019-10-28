using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AnalysisClr;

namespace ClrTest
{
    class Program
    {
        static void Main(string[] args)
        {
            EnergySP e = new EnergySP();
            int[] a=new int[2];
            a[1] = 2;
            e.Count = a;
            e.Count[1] = 3;
            a[1] = 1;
            Console.WriteLine(e.Count[1]);
            Console.ReadLine();
        }
    }
}
