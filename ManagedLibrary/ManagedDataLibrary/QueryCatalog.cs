using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
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


        public static IntPtr MakeQuerySingleRawPtr(IntPtr inputXmlPtr, IntPtr predicateFieldPtr, IntPtr predicatevaluePtr, IntPtr resultFieldPtr)
        {
            var inputXml = Marshal.PtrToStringUTF8(inputXmlPtr);
            var predicateField = Marshal.PtrToStringUTF8(predicateFieldPtr);
            var predicatevalue = Marshal.PtrToStringUTF8(predicatevaluePtr);
            var resultField = Marshal.PtrToStringUTF8(resultFieldPtr);


            var queryCatalog = new QueryCatalog();
            var result = queryCatalog.MakeQueryInternal(inputXml, predicateField, predicatevalue, resultField);

            // allocates memory away from the GC using CoTaskMemAlloc (Win) or AllocHGlobal (*nix)
            return Marshal.StringToCoTaskMemUTF8(result.FirstOrDefault());
        }


        public static IntPtr MakeQuerySingle(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string inputXml,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string predicateField,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string predicatevalue,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string resultField)
        {
            var queryCatalog = new QueryCatalog();
            var result = queryCatalog.MakeQueryInternal(inputXml, predicateField, predicatevalue, resultField);

            // allocates memory away from the GC using CoTaskMemAlloc (Win) or AllocHGlobal (*nix)
            return Marshal.StringToCoTaskMemUTF8(result.FirstOrDefault());
        }

        /*
         * Usage:
         * .NET will allocate the native memory and return a ptr **and** the number of elements
         * but not the size of each strings as we guarantee them to be null-terminated strings.
         * On the C++ side, we just get one string at a time, relying on the null and looping
         * until the give number of strings is retrieved.
        */

        public static IntPtr MakeQueryMultiRawPtr(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string inputXml,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string predicateField,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string predicatevalue,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string resultField,
            [Out] out int items)
        {
            var queryCatalog = new QueryCatalog();
            var result = queryCatalog.MakeQueryInternal(inputXml, predicateField, predicatevalue, resultField);
            items = result.Length;
            var ptr = MarshalHelper.StringArrayToPtr(result);
            return ptr;
        }


        /*
         * According to:
         * https://github.com/dotnet/runtime/issues/7315#issuecomment-365763009
         * on Windows: LPStr is ansi, LPUTF8Str is UTF-8
         * on Linux: LPStr is UTF-8, LPUTF8Str is UTF-8
         * The problems are:
         * 1. that LPArray pretends LPStr
         * 2. there is no official documentation about the responsible of the call to free
         * After several tests my best guess is that the memory should be freed (in native code):
         * - free each string allocation
         * - free the array allocation
         * 
         * =================================================================
         *  Morale: it is better to avoid this method and use the one above
         * =================================================================
         */

        public static void MakeQueryMulti_Wrong(
            [MarshalAs(UnmanagedType.LPUTF8Str)] string inputXml,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string predicateField,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string predicatevalue,
            [MarshalAs(UnmanagedType.LPUTF8Str)] string resultField,
            [MarshalAs(UnmanagedType.LPArray,
                        ArraySubType = UnmanagedType.LPStr, // this is needed by LPArray
                        SizeParamIndex =5)][Out] out string[]result,
            [Out] out int items)
        {
            var queryCatalog = new QueryCatalog();
            result = queryCatalog.MakeQueryInternal(inputXml, predicateField, predicatevalue, resultField);
            items = result.Length;
        }

        // The returned ptr must be manually freed with CoTaskMemFree (native code)
        // example: MakeQuery(xml, "year", "1987", "title")
        // - query the xml catalog in input
        // - filter the content with just the ones having year equal to 1987
        // - return the titles
        internal string[] MakeQueryInternal(string inputXml, string predicateField, string predicatevalue, string resultField)
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
                .ToArray();

            return result;
        }

        public static void ForceFree()
        {
            GC.Collect();
            GC.Collect();
            GC.Collect();
            GC.WaitForPendingFinalizers();
            GC.Collect();
        }
    }
}
