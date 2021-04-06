using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;
using ClangSharp.Interop;

using ClangVisitor.Leaves;

namespace ClangVisitor
{
    public partial class AstVisitor : IAstRootVisitor
    {
        private AstLogger _logger;
        private List<IAstLeafVisitor> _leafTable;

        public AstVisitor(AstLoggerVerbosity verbosity)
        {
            _logger = new AstLogger(verbosity);
            _leafTable = new List<IAstLeafVisitor>();
            this.Context = new AstContext(_logger);
        }

        public AstContext Context { get; }
        public string GetLog() => _logger.GetAllLog();

        public void RegisterLeafVisitor(IAstLeafVisitor leafVisitor)
        {
            _leafTable.Add(leafVisitor);
        }

        public void Visit(Cursor cursor)
        {
            Context.CursorStack.Push(cursor);
            bool isVisitorCalled = false;
            foreach (var visitor in _leafTable)
            {
                if (visitor.Filter != null && visitor.Filter(cursor))
                {
                    visitor.Visit(Context, this, cursor);
                    isVisitorCalled = true;
                }
            }

            if (!isVisitorCalled)
            {
                _logger.LogMissingLeafVisitor(Context, cursor);
            }

            if (Context.VisitMyChildren)
            {
                foreach (var child in cursor.CursorChildren)
                {
                    // recurse the children
                    Visit(child);
                }
            }
            
            Context.VisitMyChildren = true;
            Context.CursorStack.Pop();
        }

    }
}
