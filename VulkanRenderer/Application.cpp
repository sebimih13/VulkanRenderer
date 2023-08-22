#include "Application.h"

namespace VulkanRenderer
{

	Application::Application()
		: Window(WIDTH, HEIGHT, "Vulkan Renderer")
		, Pipeline("Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv")
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
