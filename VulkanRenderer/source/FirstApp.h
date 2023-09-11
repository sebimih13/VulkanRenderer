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
		/** Constructor */
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
		void drawFrame();

		VEWindow veWindow;
		VEDevice veDevice;
		VESwapChain veSwapChain;
		std::unique_ptr<VEPipeline> vePipeline;

		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

		std::unique_ptr<VEModel> veModel;
	};

} // namespace VE
