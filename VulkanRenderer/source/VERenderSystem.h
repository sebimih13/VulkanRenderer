#pragma once

#include "VEPipeline.h"
#include "VEDevice.h"
#include "VEGameObject.h"
#include "VECamera.h"

#include <memory>
#include <vector>

namespace VE
{

	class RenderSystem
	{
	public:
		RenderSystem(VEDevice& device, VkRenderPass renderPass);
		~RenderSystem();

		/** Not copyable */
		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator = (const RenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<VEGameObject>& gameObjects, const VECamera& camera);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		VEDevice& veDevice;

		std::unique_ptr<VEPipeline> vePipeline;
		VkPipelineLayout pipelineLayout;
	};

} // namespace VE