#pragma once

#include <memory>

#include "XCoreClrHelper.h"

class appcontext
{
	static std::unique_ptr<appcontext> instance;
	static std::string pubPath;

	appcontext() { }

public:

	static appcontext* getInstance()
	{
		if (instance == nullptr)
		{
			instance.reset(new appcontext());
			instance->clr = std::make_shared<raf_coreclr::XCoreClrHelper>(pubPath);
		}

		return instance.get();
	}

	static void initialize(std::string publishingPath)
	{
		pubPath = publishingPath;
	}

	void free()
	{
		instance = nullptr;
	}

	std::shared_ptr<raf_coreclr::XCoreClrHelper> clr;
};