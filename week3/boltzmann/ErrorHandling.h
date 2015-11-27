#pragma once
#include <iostream>

namespace error
{
	void crash(const char* errorMessage)
	{
		std::cout << "An unfixable error occured:\n" << errorMessage << std::endl;
		exit(-1);
	}
}