#pragma once

#include <memory>
#include <iostream>
#include <filesystem>
#include <map>

#include "XCoreClrHelper.h"
#include "xconfig.h"

class appcontext
{
	static std::unique_ptr<appcontext> instance;

	raf_tools::xconfig cfg;

	appcontext() { }

public:

	static appcontext* getInstance()
	{
		if (instance == nullptr)
		{
			instance.reset(new appcontext());
		}

		return instance.get();
	}

	void initialize(const std::string& configurationFile)
	{
		cfg.load(configurationFile, true);
		publishingPath = raf_tools::XFilesystem::make_absolutepath(cfg.get("publishPath"));
		libraryFile = cfg.get("libraryFile");
		functions = cfg.getFunctions();

		instance->clr = std::make_shared<raf_coreclr::XCoreClrHelper>(publishingPath);
	}

	void printConfiguration()
	{
		std::cout << "Executable          : " << raf_tools::XFilesystem::getExecutable() << std::endl;
		std::cout << "Executable Directory: " << raf_tools::XFilesystem::getExecutableAsString() << std::endl;

		std::cout << "Current Directory:    " << std::filesystem::current_path() << std::endl;
		std::cout << "Publishing Directory: " << publishingPath << std::endl;
		std::cout << std::endl;
	}

	void free()
	{
		instance = nullptr;
	}

	std::string publishingPath;
	std::string libraryFile;
	std::shared_ptr<raf_coreclr::XCoreClrHelper> clr;
	std::map<std::string, raf_tools::xfunction> functions;
};