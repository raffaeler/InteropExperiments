using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;
using ClangSharp.Interop;

namespace ClangVisitor.Leaves
{
    public class AstFieldDclarationVisitor : IAstLeafVisitor
    {
        public Func<Cursor, bool> Filter { get; } = c => c.CursorKind == CXCursorKind.CXCursor_FieldDecl;

        public void Visit(AstContext context, IAstRootVisitor root, Cursor cursor)
        {
            var field = cursor as FieldDecl;
            context.Logger.LogField(context, field);
        }
    }
}
