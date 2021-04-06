using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;
using ClangSharp.Interop;

namespace ClangVisitor.Leaves
{
    public interface IAstLeafVisitor
    {
        Func<Cursor, bool> Filter { get; }

        void Visit(AstContext context, IAstRootVisitor root, Cursor cursor);
    }
}
