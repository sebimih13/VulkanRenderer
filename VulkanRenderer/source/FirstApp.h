#pragma once

#include "VEWindow.h"
#include "VEPipeline.h"
#include "VEDevice.h"
#include "VESwapChain.h"
#include "VEModel.h"

#include <memory>
#include <vector>

namespace VE
{

	class FirstApp
	{
	public:
		FirstApp();
		~FirstApp();

		/** Not copyable */
		FirstApp(const FirstApp&) = delete;
		FirstApp& operator = (const FirstApp&) = delete;

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		void loadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		VEWindow veWindow;
		VEDevice veDevice;
		std::unique_ptr<VESwapChain> veSwapChain;
		std::unique_ptr<VEPipeline> vePipeline;

		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

		std::unique_ptr<VEModel> veModel;
	};

} // namespace VE
