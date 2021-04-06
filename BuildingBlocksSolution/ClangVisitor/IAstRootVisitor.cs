using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;

namespace ClangVisitor
{
    public interface IAstRootVisitor
    {
        void Visit(Cursor cursor);

        //void VisitChildren(IReadOnlyList<Cursor> cursors, bool visitChildren);
    }
}
