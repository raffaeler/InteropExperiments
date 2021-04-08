using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace ManagedComponent
{
    public sealed class QueryCatalog
    {

        static Dictionary<string, Func<XElement, string>> _projections = new Dictionary<string, Func<XElement, string>>
        {
             { "title",  e => e.Element("title").Value },
             { "artist",  e => e.Element("artist").Value },
             { "country",  e => e.Element("country").Value },
             { "company",  e => e.Element("company").Value },
             { "price",  e => e.Element("price").Value },
             { "year",  e => e.Element("year").Value },
        };


        // example: MakeQuery(xml, "year", "1987", "title")
        // - query the xml catalog in input
        // - filter the content with just the ones having year equal to 1987
        // - return the titles
        public string MakeQuerySingle(string inputXml, string predicateField, string predicatevalue, string resultField)
        {
            return MakeQueryMultiple(inputXml, predicateField, predicatevalue, resultField)
                .FirstOrDefault();
        }

        public IReadOnlyList<string> MakeQueryMultiple(string inputXml, string predicateField, string predicatevalue, string resultField)
        {
            XElement xe;
            try
            {
                xe = XElement.Parse(inputXml);
            }
            catch (Exception err)
            {
                Console.WriteLine(err.Message);
                return null;
            }

            var predicateProjection = _projections[predicateField];
            var resultProjection = _projections[resultField];

            var result = xe.Elements()
                .Where(e => predicateProjection(e) == predicatevalue)
                .Select(resultProjection)
                .ToList();

            return result;
        }
    }

}
