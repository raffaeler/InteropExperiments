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
#include <thread>

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

void invokePrint(const string& funcName)
{
    auto context = appcontext::getInstance();
    auto clr = context->clr;
    auto func = context->functions[funcName];

    using printPrototype = void(const char*);
    auto printDelegate = (printPrototype*)clr->CreateDelegate(func.assemblyName, func.className, func.methodName);
    printDelegate("Hello, world!");
}


std::string invokeQuery(const string& funcName, const string& xml,
    const string& predicateField, const string& predicateValue, const string& returnField)
{
    auto context = appcontext::getInstance();
    auto clr = context->clr;
    auto func = context->functions[funcName];

    using queryPrototype = char* (const char*, const char*, const char*, const char*);
    auto queryDelegate = (queryPrototype*)clr->CreateDelegate(func.assemblyName, func.className, func.methodName);

    char* pRes = queryDelegate(xml.c_str(), predicateField.c_str(), predicateValue.c_str(), returnField.c_str());
    std::string result(pRes);

    XUtilities::marshalFree(pRes);
    return result;
}

std::vector<std::string> invokeQueryMultiRawPtr(const string& funcName, const string& xml,
    const string& predicateField, const string& predicateValue, const string& returnField)
{
    auto context = appcontext::getInstance();
    auto clr = context->clr;
    auto func = context->functions[funcName];

    using queryPrototype = void* (const char*, const char*, const char*, const char*, int*);
    auto queryDelegate = (queryPrototype*)clr->CreateDelegate(func.assemblyName, func.className, func.methodName);

    int items;
    void* result = queryDelegate(xml.c_str(), predicateField.c_str(), predicateValue.c_str(), returnField.c_str(), &items);

    std::vector<string> vec;
    uint64_t p = 0;
    for (int i = 0; i < items; i++)
    {
        char* str = (char*)result + p;
        std::string temp(str);
        vec.push_back(temp);
        p += temp.size() + 1;
    }

    XUtilities::marshalFree(result);
    return vec;
}

// This works but it may have flaws. See the comments inside the .NET project
std::vector<std::string> invokeQueryMulti(const string& funcName, const string& xml,
    const string& predicateField, const string& predicateValue, const string& returnField)
{
    auto context = appcontext::getInstance();
    auto clr = context->clr;
    auto func = context->functions[funcName];

    using queryPrototype = void(const char*, const char*, const char*, const char*, char**, int*);
    auto queryDelegate = (queryPrototype*)clr->CreateDelegate(func.assemblyName, func.className, func.methodName);

    int items;
    char* result;
    queryDelegate(xml.c_str(), predicateField.c_str(), predicateValue.c_str(), returnField.c_str(), &result, &items);

    std::vector<string> vec;
    int p = 0;
    for (int i = 0; i < items; i++)
    {
        char* str = *((char**)result + p);
        std::string temp(str);
        vec.push_back(temp);
        p++;

        XUtilities::marshalFree(str);
    }

    XUtilities::marshalFree(result);
    return vec;
}

int main()
{
    auto context = appcontext::getInstance();
    context->initialize("config.json");
    context->printConfiguration();
    auto clr = context->clr;

    try
    {
        std::vector<std::string> trustedAssemblies;
        trustedAssemblies.push_back(context->publishingPath);
        clr->CreateAppDomain(context->libraryFile, trustedAssemblies);
        invokePrint("PrintVoid");


        auto xml = XFilesystem::load_text("cd.xml", true);
        std::string singleResult;
        std::vector<std::string> vec;


        cout << endl << "MakeQuerySingleRawPtr" << endl;
        singleResult = invokeQuery("MakeQuerySingleRawPtr", xml, "year", "1987", "title");
        cout << "    \"" << singleResult << "\"" << endl;


        cout << endl << "MakeQuerySingle" << endl;
        singleResult = invokeQuery("MakeQuerySingle", xml, "year", "1987", "title");
        cout << "    \"" << singleResult << "\"" << endl;


        cout << endl << "MakeQueryMultiRawPtr" << endl;
        vec = invokeQueryMultiRawPtr("MakeQueryMultiRawPtr", xml, "year", "1987", "title");
        for (const auto& result : vec)
        {
            cout << "    \"" << result << "\"" << endl;
        };

        cout << endl << "MakeQueryMulti_Wrong" << endl;
        vec = invokeQueryMulti("MakeQueryMulti_Wrong", xml, "year", "1987", "title");
        for (const auto& result : vec)
        {
            cout << "    \"" << result << "\"" << endl;
        };
    }
    catch (const runtime_error& err)
    {
        auto msg = "Error creating the CLR and/or invoking the .NET Code"s + std::string(err.what());
    }

    cin.get();

    context->free();
    return 0;
}
