#include "FirstApp.h"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main()
{
	try
	{
		VE::FirstApp app = {};
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
