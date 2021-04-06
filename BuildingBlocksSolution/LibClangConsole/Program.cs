using System;
using System.IO;
using System.Threading.Tasks;

using ClangVisitor;
using ClangVisitor.Leaves;

namespace LibClangConsole
{
    class Program
    {
        static async Task Main(string[] args)
        {
            await new Program().Start();
        }

        private async Task Start()
        {
            var sourceFilename = Path.GetFullPath(@"Samples/Demo1.txt");
            if (!File.Exists(sourceFilename))
            {
                Console.WriteLine($"File not found: {sourceFilename}");
                return;
            }

            AstVisitor visitor = new AstVisitor(AstLoggerVerbosity.Verbose);
            visitor.RegisterLeafVisitor(new AstTranslationUnitVisitor());
            visitor.RegisterLeafVisitor(new AstMacroDefinitionVisitor());
            visitor.RegisterLeafVisitor(new AstClassDeclarationVisitor());
            visitor.RegisterLeafVisitor(new AstFunctionDeclarationVisitor());
            visitor.RegisterLeafVisitor(new AstAccessDeclarationVisitor());
            visitor.RegisterLeafVisitor(new AstFieldDclarationVisitor());

            var parser = new SimpleParser();
            await parser.Parse(sourceFilename, visitor);
            Console.WriteLine(parser.DiagnosticReport);
            Console.WriteLine(visitor.GetLog());


            await Task.Delay(0);
        }
    }
}
