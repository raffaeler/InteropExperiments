using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;

namespace ClangVisitor
{
    public class AstContext
    {
        public AstContext(AstLogger logger)
        {
            this.Logger = logger;
            this.CursorStack = new Stack<Cursor>();
            this.VisitMyChildren = true;    // default value
        }

        public AstLogger Logger { get; }
        public IAstCustomContext CustomContext { get; set; }
        public Stack<Cursor> CursorStack { get; }

        /// <summary>
        /// A leaf can set this to false to tell the root to not visit its children
        /// It is automatically reset to true from the root in order.
        /// This means that it must be explicitly set every time in order not to visit the subtree
        /// </summary>
        public bool VisitMyChildren { get; set; }
    }
}
