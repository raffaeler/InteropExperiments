using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Linq.Expressions;
using System.Xml.Linq;

namespace EmptyApp
{
    class Program
    {
        static void Main(string[] args)
        {
            //test();
            Console.WriteLine("Hello World!");
        }

        static Dictionary<string, Func<XElement, string>> _projections = new Dictionary<string, Func<XElement, string>>
        {
             { "title",  e => e.Element("title").Value },
             { "artist",  e => e.Element("artist").Value },
             { "country",  e => e.Element("country").Value },
             { "company",  e => e.Element("company").Value },
             { "price",  e => e.Element("price").Value },
             { "year",  e => e.Element("year").Value },
        };

        private static void test()
        {
            var xe = XElement.Parse(File.ReadAllText("H:\\a\\cd.xml"));

            string predicateField = "year";
            string predicatevalue = "1987";
            string resultField = "title";

            var predicateProjection = _projections[predicateField];
            var resultProjection = _projections[resultField];

            var result = xe.Elements()
                .Where(e => predicateProjection(e) == predicatevalue)
                .Select(resultProjection)
                .ToArray();
        }


    }

}
