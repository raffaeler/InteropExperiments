#pragma once

#include "json.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>

#include "Xfilesystem.h"
#include "XUtilities.h"

namespace raf_tools
{
	using json = nlohmann::json;
	using namespace std;

	class xfunction
	{
	public:
		std::string name;	// familiar, unique name
		//std::string libraryFile;	// loaded from .NET
		std::string assemblyName;
		std::string className;
		std::string methodName;
	};

	class xconfig
	{
		//xconfig(const xconfig&) = delete;
		const xconfig& operator=(const xconfig&) = delete;

		json _root;

	public:
		//xconfig(xconfig&&) = default;
		explicit xconfig()
		{
		}

		virtual ~xconfig()
		{
		}

		void load(const std::string& filename, bool locateInExeDirectory)
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
			stream >> _root;
		}

		void save(const std::string& filename)
		{
			std::ofstream stream(filename);
			stream << std::setw(4) << _root << std::endl;
		}

		std::string get(const std::string& key) const
		{
			return _root[key].get<std::string>();
		}

		bool getBool(const std::string& key) const
		{
			return _root[key].get<bool>();
		}

		map<std::string, xfunction> getFunctions()
		{
			map<std::string, xfunction> res;
			auto funcs = _root["functions"s];
			std::for_each(begin(funcs), end(funcs), [&res] (const json & item)
			{
				xfunction func;
				func.name = item["name"].get<std::string>();
				func.assemblyName = item["assemblyName"].get<std::string>();
				func.className = item["className"].get<std::string>();
				func.methodName = item["methodName"].get<std::string>();
				
				res[func.name] = func;
			});

			return res;
		}


		void print()
		{
			std::cout << _root.dump(4) << std::endl;
		}
	};
}