using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;
using ClangSharp.Interop;

namespace ClangVisitor.Leaves
{
    public class AstFunctionDeclarationVisitor : IAstLeafVisitor
    {
        private CX_DeclKind[] Declarations = new CX_DeclKind[]
        {
            CX_DeclKind.CX_DeclKind_Function,
            CX_DeclKind.CX_DeclKind_CXXMethod,
            CX_DeclKind.CX_DeclKind_CXXConstructor,
            CX_DeclKind.CX_DeclKind_CXXDestructor,
            CX_DeclKind.CX_DeclKind_CXXConversion,
        };


        public Func<Cursor, bool> Filter => c => c is Decl declaration && Declarations.Contains(declaration.Kind);

        public void Visit(AstContext context, IAstRootVisitor root, Cursor cursor)
        {
            context.Logger.LogMethod(context, cursor as CXXMethodDecl);
        }
    }
}
