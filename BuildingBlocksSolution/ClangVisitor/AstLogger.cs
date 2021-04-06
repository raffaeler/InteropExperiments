using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;

namespace ClangVisitor
{
    public class AstLogger
    {
        private StringBuilder _sb;

        public AstLogger(AstLoggerVerbosity verbosity)
        {
            this.Verbosity = verbosity;
            _sb = new StringBuilder();
        }

        public AstLoggerVerbosity Verbosity { get; }

        public void LogLeaf(AstContext context, Cursor cursor)
        {
            _sb.AppendLine($"Node [kind={cursor.CursorKind}, {cursor.CursorKindSpelling}]");
        }

        public void LogMissingLeafVisitor(AstContext context, Cursor cursor)
        {
            if (Verbosity > AstLoggerVerbosity.ErrorOnly)
            {
                _sb.AppendLine($"No visitor for cursor Kind=[{cursor.CursorKind}, {cursor.CursorKindSpelling}] Type={cursor.GetType().Name}]");
            }
        }

        public void LogMacroValue(AstContext context, Cursor cursor, string value)
        {
            if (Verbosity >= AstLoggerVerbosity.Verbose)
            {
                _sb.AppendLine($"Macro Kind=[{cursor.CursorKind}] Value={value}]");
            }
        }

        public void LogField(AstContext context, FieldDecl cursor)
        {
            if (Verbosity >= AstLoggerVerbosity.Verbose)
            {
                _sb.AppendLine($"Field Kind=[{cursor.CursorKind}] {cursor.Type} {cursor.Name}]");
            }
        }

        public void LogMethod(AstContext context, CXXMethodDecl cursor)
        {
            if (Verbosity >= AstLoggerVerbosity.Verbose)
            {
                _sb.AppendLine($"Method Kind=[{cursor.CursorKind}] {cursor.Type} {cursor.Name} {cursor.UnderlyingDecl} {cursor.Visibility} {cursor.DeclaredReturnType}]");
            }
        }


        public string GetAllLog() => _sb.ToString();
    }
}
