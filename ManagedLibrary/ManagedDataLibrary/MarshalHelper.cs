using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ManagedDataLibrary
{
    public static class MarshalHelper
    {
        public static IntPtr StringArrayToPtr(string[] stringArray)
        {
            var blobResult = CreateFlatUTF8StringArray(stringArray);
            var ptrResult = Marshal.AllocCoTaskMem(blobResult.Length);
            Marshal.Copy(blobResult, 0, ptrResult, blobResult.Length);
            return ptrResult;
        }

        public static byte[] CreateFlatUTF8StringArray(string[] stringArray)
        {
            var totalSize = stringArray.Sum(s => Encoding.UTF8.GetByteCount(s));
            totalSize += stringArray.Length;    // null chars
            byte[] blob = new byte[totalSize];
            Span<byte> result = new(blob);

            var offset = 0;
            foreach (var s in stringArray)
            {
                var slice = result.Slice(offset);
                offset += Encoding.UTF8.GetBytes(s, slice) + 1;
            }

            return blob;
        }

    }
}
