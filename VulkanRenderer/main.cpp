#include "Application.h"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main()
{
	try
	{
		VulkanRenderer::Application App;
		App.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
