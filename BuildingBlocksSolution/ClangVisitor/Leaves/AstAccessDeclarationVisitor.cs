using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;
using ClangSharp.Interop;

namespace ClangVisitor.Leaves
{
    public class AstAccessDeclarationVisitor : IAstLeafVisitor
    {
        private CX_DeclKind[] Declarations = new CX_DeclKind[]
        {
        };


        public Func<Cursor, bool> Filter => c => c.CursorKind == CXCursorKind.CXCursor_LastDecl;

        public void Visit(AstContext context, IAstRootVisitor root, Cursor cursor)
        {
            var spec = cursor as AccessSpecDecl;
            context.Logger.LogLeaf(context, cursor);
        }
    }
}
