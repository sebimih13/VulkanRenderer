#pragma once

// TODO : implement interface for apps

#include "../VEWindow.h"
#include "../VEPipeline.h"
#include "../VEDevice.h"
#include "../VESwapChain.h"
#include "../VEModel.h"

#include <memory>
#include <vector>

namespace VE
{

	class SierpinskiTriangleApp
	{
	public:
		SierpinskiTriangleApp();
		~SierpinskiTriangleApp();

		/** Not copyable */
		SierpinskiTriangleApp(const SierpinskiTriangleApp&) = delete;
		SierpinskiTriangleApp& operator = (const SierpinskiTriangleApp&) = delete;

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

		/** time variables */
		static float deltaTime;		// time between current frame and last frame
		static float lastFrame;		// time of last frame
	};

} // namespace VE
