#include "Application.h"

namespace VulkanRenderer
{

	Application::Application()
		: Window(WIDTH, HEIGHT, "Vulkan Renderer")
	{
		
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (!Window.ShouldClose())
		{
			glfwPollEvents();
		}
	}

} // namespace VulkanRenderer
