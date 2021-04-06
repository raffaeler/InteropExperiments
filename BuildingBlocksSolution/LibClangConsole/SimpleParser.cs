using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ClangSharp;
using ClangSharp.Interop;

using ClangVisitor;

namespace LibClangConsole
{
    public class SimpleParser : IDisposable
    {
        private bool _disposed;
        private const string _language = "c++";
        private const string _std = "c++17";
        private string[] _includeDirectories = new string[] { };
        private string[] _defineMacros = new string[] { };
        private string[] _additionalArgs = new string[] { };

        private readonly CXIndex _index;
        private string[] _clangArgs = new string[]
        {
            $"--language={_language}",
            $"--std={_std}",
            "-Wno-pragma-once-outside-header",
        };


        public SimpleParser()
        {
            _index = CXIndex.Create();

            _clangArgs = _clangArgs.Concat(_includeDirectories.Select(x => "--include-directory=" + x)).ToArray();
            _clangArgs = _clangArgs.Concat(_defineMacros.Select(x => "--define-macro=" + x)).ToArray();
            _clangArgs = _clangArgs.Concat(_additionalArgs).ToArray();
        }

        public CXIndex IndexHandle => _index;
        public CXTranslationUnit TranslationUnitHandle { get; private set; }
        public string DiagnosticReport { get; private set; }
        public TranslationUnit TranslationUnit { get; private set; }

        private CXTranslationUnit_Flags TranslationUnitFlags { get; set; }

        public async Task Parse(string filename, AstVisitor visitor)
        {
            TranslationUnitFlags = CXTranslationUnit_Flags.CXTranslationUnit_None;
            TranslationUnitFlags |= CXTranslationUnit_Flags.CXTranslationUnit_IncludeAttributedTypes;               // Include attributed types in CXType
            TranslationUnitFlags |= CXTranslationUnit_Flags.CXTranslationUnit_VisitImplicitAttributes;              // Implicit attributes should be 
            TranslationUnitFlags |= CXTranslationUnit_Flags.CXTranslationUnit_DetailedPreprocessingRecord;          // GenerateMacroBindings

            var error = CXTranslationUnit.TryParse(_index, filename, _clangArgs,
                Array.Empty<CXUnsavedFile>(), TranslationUnitFlags, out CXTranslationUnit TranslationUnitHandle);

            if (error != CXErrorCode.CXError_Success)
                throw new Exception($"Parsing error on {filename}: {error}");

            DiagnosticReport = DumpDiagnostics(filename);

            try
            {
                TranslationUnit = TranslationUnit.GetOrCreate(TranslationUnitHandle);
                await InternalParse(TranslationUnit, filename, _clangArgs, TranslationUnitFlags, visitor);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
        }

        private Task InternalParse(TranslationUnit translationUnit, string filename, string[] clangArgs, CXTranslationUnit_Flags translationUnitFlags,
            AstVisitor visitor)
        {
            var translationUnitHandle = translationUnit.Handle;
            var file = translationUnitHandle.GetFile(filename);
            var fileContents = translationUnitHandle.GetFileContents(file, out var size);

            if (false)
            {
                var translationFlags = translationUnitFlags & ~CXTranslationUnit_Flags.CXTranslationUnit_DetailedPreprocessingRecord;
                var handle = CXTranslationUnit.Parse(IndexHandle, filename, clangArgs, Array.Empty<CXUnsavedFile>(), translationFlags);
                using var nestedTranslationUnit = TranslationUnit.GetOrCreate(handle);
                //Visit(nestedTranslationUnit.TranslationUnitDecl);
                visitor.Visit(nestedTranslationUnit.TranslationUnitDecl);
            }
            else
            {
                //Visit(translationUnit.TranslationUnitDecl);
                visitor.Visit(translationUnit.TranslationUnitDecl);
            }


            return Task.CompletedTask;
        }

        private void Visit(Cursor cursor)
        {
            //var currentContext = _context.AddLast(cursor);

            //if (cursor is Attr attr)
            //{
            //    VisitAttr(attr);
            //}
            //else if (cursor is Decl decl)
            //{
            //    VisitDecl(decl);
            //}
            //else if (cursor is Ref @ref)
            //{
            //    VisitRef(@ref);
            //}
            //else if (cursor is Stmt stmt)
            //{
            //    VisitStmt(stmt);
            //}
            //else
            //{
            //    AddDiagnostic(DiagnosticLevel.Error, $"Unsupported cursor: '{cursor.CursorKindSpelling}'. Generated bindings may be incomplete.", cursor);
            //}

            //Debug.Assert(_context.Last == currentContext);
            //_context.RemoveLast();
        }



        private string DumpDiagnostics(string filename)
        {
            StringBuilder sb = new();
            sb.AppendLine($"Filename: {filename}");
            if (TranslationUnitHandle != null && TranslationUnitHandle.NumDiagnostics > 0)
            {
                for (uint i = 0; i < TranslationUnitHandle.NumDiagnostics; i++)
                {
                    using var diag = TranslationUnitHandle.GetDiagnostic(i);
                    sb.AppendLine(diag.Format(CXDiagnostic.DefaultDisplayOptions).ToString());
                }
            }

            return sb.ToString();
        }

        public void Close()
        {
            if (TranslationUnit != null)
            {
                TranslationUnit.Dispose();
                TranslationUnit = null;
            }
        }

        #region Dispose Pattern
        private void Dispose(bool isDisposing)
        {
            //Debug.Assert(_outputBuilder is null);

            if (_disposed)
            {
                return;
            }
            _disposed = true;

            if (isDisposing)
            {
                Close();
            }
            _index.Dispose();
        }

        public void Dispose()
        {
            Dispose(isDisposing: true);
            GC.SuppressFinalize(this);
        }
        #endregion



    }
}
