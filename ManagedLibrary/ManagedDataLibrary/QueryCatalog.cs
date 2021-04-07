using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace ManagedDataLibrary
{
    public class QueryCatalog
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


        public static IntPtr MakeQuery(IntPtr inputXmlPtr, IntPtr predicateFieldPtr, IntPtr predicatevaluePtr, IntPtr resultFieldPtr)
        {
            var inputXml = Marshal.PtrToStringUTF8(inputXmlPtr);
            var predicateField = Marshal.PtrToStringUTF8(predicateFieldPtr);
            var predicatevalue = Marshal.PtrToStringUTF8(predicatevaluePtr);
            var resultField = Marshal.PtrToStringUTF8(resultFieldPtr);


            var queryCatalog = new QueryCatalog();
            var result = queryCatalog.MakeQueryInternal(inputXml, predicateField, predicatevalue, resultField);

            // allocates memory away from the GC using CoTaskMemAlloc (Win) or AllocHGlobal (*nix)
            return Marshal.StringToCoTaskMemUTF8(result);
        }

        // example: MakeQuery(xml, "year", "1987", "title")
        // - query the xml catalog in input
        // - filter the content with just the ones having year equal to 1987
        // - return the titles
        internal string MakeQueryInternal(string inputXml, string predicateField, string predicatevalue, string resultField)
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
                .FirstOrDefault();

            return result;
        }
    }
}
