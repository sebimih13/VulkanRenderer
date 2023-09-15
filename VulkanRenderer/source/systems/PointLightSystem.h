#pragma once

#include "../VEPipeline.h"
#include "../VEDevice.h"
#include "../VEGameObject.h"
#include "../VECamera.h"
#include "../VEFrameInfo.h"

#include <memory>
#include <vector>
#include <map>

namespace VE
{

	class PointLightSystem
	{
	public:
		PointLightSystem(VEDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		/** Not copyable */
		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator = (const PointLightSystem&) = delete;

		void update(FrameInfo& frameInfo, GlobalUBO& ubo);
		void render(FrameInfo& frameInfo);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		VEDevice& veDevice;

		std::unique_ptr<VEPipeline> vePipeline;
		VkPipelineLayout pipelineLayout;
	};

} // namespace VE
