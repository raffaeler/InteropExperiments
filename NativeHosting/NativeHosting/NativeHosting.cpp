// Official hosting example:
// https://github.com/dotnet/samples/blob/master/core/hosting/HostWithCoreClrHost/src/SampleHost.cpp
//

// This hosting code one is different from the official one
// questions to Raffaele Rialdi @raffaeler

//#define WIN64

//#define __STDC_WANT_LIB_EXT1__ 1
#include <string>
#include <iostream>
#include <memory>
#include <future>
#include <array>
#include <filesystem>

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

void makeCall(const xconfig& cfg)
{
	auto context = appcontext::getInstance();
	auto clr = context->clr;

	//auto assemblyName = "DemoLibrary"s;
	//auto className = "DemoLibrary.SimpleClass";
	//auto methodName = "Print";
	//auto message = u"Hello, world";

	auto assemblyName = cfg.get("assemblyName");
	auto className = cfg.get("className");
	auto methodName = cfg.get("methodName");
	auto msg = cfg.get("message");// XUtilities::ToU16(cfg.get("message"));
	auto message = msg.c_str();

	using DelProto = void(void*);
	auto del = (DelProto*)clr->CreateDelegate(assemblyName, className, methodName);
	del((void*)message);
}


int main()
{
	auto cfg = xconfig::load("config.json", true);
	auto lib = cfg.get("libraryFile");
	auto pubPath = XFilesystem::make_absolutepath(cfg.get("publishPath"));
	auto btrue = cfg.getBool("boolValueTrue");
	auto bfalse = cfg.getBool("boolValueFalse");

	//cout << lib << endl;
	//cout << btrue << endl;
	//cout << bfalse << endl;

	std::cout << "Executable          : " << XFilesystem::getExecutable() << endl;
	std::cout << "Executable Directory: " << XFilesystem::getExecutableAsString() << endl;

	std::cout << "Current Directory:    " << current_path() << std::endl;
	std::cout << "Publishing Directory: " << pubPath << endl;
	std::cout << endl;
	//std::cout << "=== config ===" << std::endl;
	//cfg.print();
	//std::cout << "==============" << std::endl;

	appcontext::initialize(pubPath);
	auto context = appcontext::getInstance();
	auto clr = context->clr;

	try
	{
		std::vector<std::string> trustedAssemblies;
		trustedAssemblies.push_back(pubPath);
		clr->CreateAppDomain(lib, trustedAssemblies);
		makeCall(cfg);
	}
	catch (const runtime_error& err)
	{
		auto msg = "Error creating the CLR and/or invoking the .NET Code"s + std::string(err.what());
	}

	cin.get();

	context->free();
	return 0;
}
