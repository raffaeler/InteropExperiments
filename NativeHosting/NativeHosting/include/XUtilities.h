#pragma once
// X-platform utilities

#include <vector>
#include <string>
#include <sstream>
#include <exception>
#include <algorithm>

#include <cstring>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

namespace raf_tools
{
    using namespace std::string_literals;

    class XUtilities
    {
        static bool caseInsensitiveStringCompare(const std::string& str1, const std::string& str2)
        {
            std::string str1Cpy(str1);
            std::string str2Cpy(str2);
            std::transform(str1Cpy.begin(), str1Cpy.end(), str1Cpy.begin(), ::tolower);
            std::transform(str2Cpy.begin(), str2Cpy.end(), str2Cpy.begin(), ::tolower);
            return (str1Cpy == str2Cpy);
        }

    public:
#if defined(WIN32) || defined(WIN64)
        static std::string PathSeparator() { return "\\"s; }
#else
        static std::string PathSeparator() { return "/"s; }
#endif

        static std::u16string ToU16(const std::string& str)
        {
            return std::u16string(begin(str), end(str));
        }

        //static std::u16string ToU16(const std::string& source)
        //{
        //	// <codecvt> not supported from old gcc versions (RPi)
        //	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        //	std::u16string dest = convert.from_bytes(source);
        //	return dest;
        //}

        //static std::string ToU8(const std::u16string& source)
        //{
        //	// <codecvt> not supported from old gcc versions (RPi)
        //	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
        //	std::string dest = convert.to_bytes(source);
        //	return dest;
        //}

        static std::string join(const std::vector<std::string>& elements, std::string delimiter)
        {
            std::stringstream ss;
            auto size = elements.size();
            auto lastIndex = size - 1;

            for (size_t i = 0; i < size; ++i)
            {
                ss << elements[i];

                if (i != lastIndex)
                {
                    ss << delimiter;
                }
            }

            return ss.str();
        }

        static const char* GetEnvValueBoolean(const char* envVariable)
        {
            static const char* valfalse = "false";
            static const char* valtrue = "true";
            static const int buflen = 7;

            /* getenv_s is not supported in Clang on Linux */
            //size_t requiredSize = buflen;
            //char envValue[buflen];
            //memset(envValue, 0, buflen);
            //if (getenv_s(&requiredSize, envValue, buflen, envVariable) != 0) return valfalse;

            //// CoreCLR expects strings "true" and "false" instead of "1" and "0".
            //return (std::strcmp(envValue, "1") == 0 || caseInsensitiveStringCompare(envValue, "true") == 0) ? valtrue : valfalse;

#pragma warning( push )
#pragma warning( disable : 4996 )
            char* envValue = getenv(envVariable);
#pragma warning( pop )
            if (envValue == nullptr) return valfalse;
            if (envValue[0] == '1') return valtrue;
            return valfalse;
        }

    };
}