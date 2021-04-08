#pragma once

#include <iostream>
#include <fstream>
#include <iterator>
#include <filesystem>

class Utilities
{
public:
    static std::string load_text(const std::string& filename, const std::filesystem::path& dir)
    {
        std::filesystem::path full = dir / filename;

        std::ifstream stream(full);
        std::string str((std::istreambuf_iterator<char>(stream)),
            std::istreambuf_iterator<char>());

        return str;
    }
};