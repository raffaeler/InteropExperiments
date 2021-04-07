#pragma once
#include <iostream>
#include <filesystem>

#if defined (WIN32) || defined (WIN64)
#include <Windows.h>
#define MAX_PATH_DEFAULT    MAX_PATH
#endif

//
// some utilities using the newest <filesystem> library
//

namespace raf_tools
{
    using namespace std::filesystem;

    class XFilesystem
    {
    public:

        static std::string make_absolutepath(const std::string& p)
        {
            std::filesystem::path p1 = p;
            if (p1.is_relative())
            {
                std::filesystem::path res = std::filesystem::current_path();
                res /= p;
                return std::filesystem::canonical(res).generic_string();
            }

            return std::filesystem::canonical(p1).generic_string();
        }

        static path getExecutable()
        {
#if defined (WIN32) || defined (WIN64)
            char realPath[MAX_PATH_DEFAULT];
            if (::GetModuleFileNameA(NULL, realPath, MAX_PATH_DEFAULT) != 0)
            {
                path exePath(realPath);
                return exePath;
            }
            else
            {
                return path();
            }
#else

            path exePath(std::filesystem::canonical("/proc/self/exe"));
            return exePath;
#endif
        }

        static path getExecutablePath()
        {
            return getExecutable().remove_filename();
        }

        static std::string getExecutableAsString()
        {
            return getExecutablePath().string();
        }

        static std::string load_text(const std::string& filename, bool locateInExeDirectory)
        {
            std::filesystem::path full;

            if (locateInExeDirectory)
            {
                full = XFilesystem::getExecutablePath() /= filename;
            }
            else
            {
                full = filename;
            }

            std::ifstream stream(full);
            std::string str((std::istreambuf_iterator<char>(stream)),
                std::istreambuf_iterator<char>());

            return str;
        }

    };
}