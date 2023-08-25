#include "Application.h"

#include <stdexcept>
#include <array>

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
			DrawFrame();
		}

		vkDeviceWaitIdle(Device.device());
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
		CommandBuffers.resize(SwapChain.imageCount());

		VkCommandBufferAllocateInfo AllocInfo = {};
		AllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		AllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		AllocInfo.commandPool = Device.getCommandPool();
		AllocInfo.commandBufferCount = static_cast<uint32_t>(CommandBuffers.size());

		if (vkAllocateCommandBuffers(Device.device(), &AllocInfo, CommandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers");
		}

		for (int i = 0; i < CommandBuffers.size(); ++i)
		{
			VkCommandBufferBeginInfo BeginInfo = {};
			BeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(CommandBuffers[i], &BeginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording command buffer");
			}

			VkRenderPassBeginInfo RenderPassInfo = {};
			RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			RenderPassInfo.renderPass = SwapChain.getRenderPass();
			RenderPassInfo.framebuffer = SwapChain.getFrameBuffer(i);

			RenderPassInfo.renderArea.offset = { 0, 0 };
			RenderPassInfo.renderArea.extent = SwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> ClearValues = {};
			ClearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			ClearValues[1].depthStencil = { 1.0f, 0 };

			RenderPassInfo.clearValueCount = static_cast<uint32_t>(ClearValues.size());
			RenderPassInfo.pClearValues = ClearValues.data();

			vkCmdBeginRenderPass(CommandBuffers[i], &RenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			Pipeline->Bind(CommandBuffers[i]);
			vkCmdDraw(CommandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(CommandBuffers[i]);

			if (vkEndCommandBuffer(CommandBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to record command buffer");
			}
		}
	}

	void Application::DrawFrame()
	{
		uint32_t ImageIndex;
		auto Result = SwapChain.acquireNextImage(&ImageIndex);

		if (Result != VK_SUCCESS && Result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image");
		}

		Result = SwapChain.submitCommandBuffers(&CommandBuffers[ImageIndex], &ImageIndex);
		if (Result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image");
		}
	}

} // namespace VulkanRenderer
