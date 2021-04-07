// Official hosting example:
// https://github.com/dotnet/samples/blob/master/core/hosting/HostWithCoreClrHost/src/SampleHost.cpp
//

// This hosting code one is different from the official one
// questions to Raffaele Rialdi @raffaeler


// The following #define is to use the secure CRT library defined in the C++ standard
// Unfortunately the safe getenv is not available in Clang/Linux at the moment
//#define __STDC_WANT_LIB_EXT1__ 1
#include <string>
#include <iostream>
#include <memory>
#include <future>
#include <array>
#include <filesystem>
#include <functional>

#include "include/xconfig.h"
#include "include/appcontext.h"

#include "include/Xfilesystem.h"

#include "NativeHosting.h"

using namespace std;
using namespace std::filesystem;
using namespace raf_tools;
using namespace raf_coreclr;

// declaration of static members
std::unique_ptr<appcontext> appcontext::instance;
std::string appcontext::pubPath;

void invokePrint(const xfunction& func)
{
	auto context = appcontext::getInstance();
	auto clr = context->clr;

	using printPrototype = void(const char*);
	auto printDelegate = (printPrototype*)clr->CreateDelegate(func.assemblyName, func.className, func.methodName);
	printDelegate("Hello, world!");
}


void invokeQuery(const xfunction& func, const string& xml,
	const string& predicateField, const string& predicateValue, const string& returnField,
	std::function<void(std::string)> resultFunc)
{
	auto context = appcontext::getInstance();
	auto clr = context->clr;

	using queryPrototype = char*(const char*, const char*, const char*, const char*);
	auto queryDelegate = (queryPrototype*)clr->CreateDelegate(func.assemblyName, func.className, func.methodName);

	char* pRes = queryDelegate(xml.c_str(), predicateField.c_str(), predicateValue.c_str(), returnField.c_str());
	resultFunc(pRes);

	// free the allocation
	XUtilities::marshalFree(pRes);
}

int main()
{
	auto sep = std::filesystem::path::preferred_separator;

	auto cfg = xconfig::load("config.json", true);
	auto pubPath = XFilesystem::make_absolutepath(cfg.get("publishPath"));
	auto libraryFile = cfg.get("libraryFile");

	std::cout << "Executable          : " << XFilesystem::getExecutable() << endl;
	std::cout << "Executable Directory: " << XFilesystem::getExecutableAsString() << endl;

	std::cout << "Current Directory:    " << current_path() << std::endl;
	std::cout << "Publishing Directory: " << pubPath << endl;
	std::cout << endl;
	//std::cout << "=== config ===" << std::endl;
	//cfg.print();
	//std::cout << "==============" << std::endl;

	auto funcs = cfg.getFunctions();

	appcontext::initialize(pubPath);
	auto context = appcontext::getInstance();
	auto clr = context->clr;

	try
	{
		std::vector<std::string> trustedAssemblies;
		trustedAssemblies.push_back(pubPath);
		clr->CreateAppDomain(libraryFile, trustedAssemblies);
		invokePrint(funcs["PrintVoid"]);


		auto xml = XFilesystem::load_text("cd.xml", true);

		invokeQuery(funcs["MakeQuerySingle"], xml, "year", "1987", "title", [](const string& result)
			{
				cout << ".NET Query result is \"" << result << "\"" << endl;
			});
	}
	catch (const runtime_error& err)
	{
		auto msg = "Error creating the CLR and/or invoking the .NET Code"s + std::string(err.what());
	}

	cin.get();

	context->free();
	return 0;
}
