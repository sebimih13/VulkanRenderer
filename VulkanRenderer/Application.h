#pragma once

#include "VulkanWindow.h"
#include "VulkanPipeline.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"

#include <memory>
#include <vector>

namespace VulkanRenderer
{

	class Application
	{
	public:
		/** Constructor */
		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator = (const Application&) = delete;

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void Run();

	private:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCommandBuffers();
		void DrawFrame();

		VulkanWindow Window;
		VulkanDevice Device;
		VulkanSwapChain SwapChain;

		std::unique_ptr<VulkanPipeline> Pipeline;

		VkPipelineLayout PipelineLayout;
		std::vector<VkCommandBuffer> CommandBuffers;
	};

} // namespace VulkanRenderer
