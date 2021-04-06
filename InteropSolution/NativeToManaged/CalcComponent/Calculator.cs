using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CalcComponent
{
    public sealed class Calculator
    {
        public int Version { get; } = 2;

        public double Add(double x, double y) => x + y;
        public double Sub(double x, double y) => x - y;

        public string Greet(string name) => $"Hello {name}";
    }
}
