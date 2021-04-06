using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;

namespace ClangVisitor.Leaves
{
    public class AstTranslationUnitVisitor : IAstLeafVisitor
    {
        public Func<Cursor, bool> Filter { get; } = c => c is TranslationUnitDecl translationUnit;

        public void Visit(AstContext context, IAstRootVisitor root, Cursor cursor)
        {
            TranslationUnitDecl decl = cursor as TranslationUnitDecl;
        }
    }
}
