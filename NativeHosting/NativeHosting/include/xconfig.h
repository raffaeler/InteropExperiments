#pragma once

#include "json.hpp"
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "Xfilesystem.h"
#include "XUtilities.h"

namespace raf_tools
{
	using json = nlohmann::json;

	class xconfig
	{
		//xconfig(const xconfig&) = delete;
		const xconfig& operator=(const xconfig&) = delete;

		json _root;

	public:
		explicit xconfig()
		{
		}

		virtual ~xconfig()
		{
		}

		static xconfig load(const std::string& filename, bool locateInExeDirectory)
		{
			xconfig cfg;
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
			stream >> cfg._root;

			return cfg;
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

		void print()
		{
			std::cout << _root.dump(4) << std::endl;
		}
	};
}