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
            e.Count[0] = 1;
            Console.WriteLine(e.Count[0]);
        }
    }
}
