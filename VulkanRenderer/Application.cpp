#include "Application.h"

namespace VulkanRenderer
{

	Application::Application()
		: Window(WIDTH, HEIGHT, "Vulkan Renderer")
		, Device(Window)
		, Pipeline(Device, VulkanPipeline::DefaultPipelineConfigInfo(WIDTH, HEIGHT), "Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv")
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
