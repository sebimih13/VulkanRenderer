#pragma once

#include "VEWindow.h"
#include "VEDevice.h"
#include "VESwapChain.h"

#include <memory>
#include <vector>

namespace VE
{

	class VERenderer
	{
	public:
		VERenderer(VEWindow& window, VEDevice& device);
		~VERenderer();

		/** Not copyable */
		VERenderer(const VERenderer&) = delete;
		VERenderer& operator = (const VERenderer&) = delete;

		VkCommandBuffer beginFrame();
		void endFrame();

		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		inline const bool isFrameInProgress() const { return isFrameStarted; }
		inline VkRenderPass getSwapChainRenderPass() const { return veSwapChain->getRenderPass(); }
		inline float getAspectRatio() const { return veSwapChain->extentAspectRatio(); }
		VkCommandBuffer getCurrentCommandBuffer() const;
		inline const int getFrameIndex() const;

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		VEWindow& veWindow;
		VEDevice& veDevice;

		std::unique_ptr<VESwapChain> veSwapChain;

		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex;
		bool isFrameStarted;
	};

} // namespace VE
