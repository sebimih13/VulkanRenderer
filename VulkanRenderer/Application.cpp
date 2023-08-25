#include "Application.h"

#include <stdexcept>

namespace VulkanRenderer
{

	Application::Application()
		: Window(WIDTH, HEIGHT, "Vulkan Renderer")
		, Device(Window)
		, SwapChain(Device, Window.GetExtent())
	{
		CreatePipelineLayout();
		CreatePipeline();
		CreateCommandBuffers();
	}

	Application::~Application()
	{
		vkDestroyPipelineLayout(Device.device(), PipelineLayout, nullptr);
	}

	void Application::Run()
	{
		while (!Window.ShouldClose())
		{
			glfwPollEvents();
		}
	}

	void Application::CreatePipelineLayout()
	{
		VkPipelineLayoutCreateInfo PipelineLayoutInfo = {};
		PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		PipelineLayoutInfo.setLayoutCount = 0;
		PipelineLayoutInfo.pSetLayouts = nullptr;
		PipelineLayoutInfo.pushConstantRangeCount = 0;
		PipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(Device.device(), &PipelineLayoutInfo, nullptr, &PipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void Application::CreatePipeline()
	{
		PipelineConfigInfo PipelineConfig = VulkanPipeline::DefaultPipelineConfigInfo(SwapChain.width(), SwapChain.height());
		PipelineConfig.RenderPass = SwapChain.getRenderPass();
		PipelineConfig.PipelineLayout = PipelineLayout;
		Pipeline = std::make_unique<VulkanPipeline>(Device, PipelineConfig, "Shaders/simple_shader.vert.spv", "Shaders/simple_shader.frag.spv");
	}

	void Application::CreateCommandBuffers()
	{

	}

	void Application::DrawFrame()
	{

	}

} // namespace VulkanRenderer
