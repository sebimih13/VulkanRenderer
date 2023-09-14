#pragma once

#include "VECamera.h"
#include "VEGameObject.h"

#include <vulkan/vulkan.h>

namespace VE
{

	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VECamera& camera;
		VkDescriptorSet globalDescriptorSet;
		VEGameObject::Map& gameObjects;
	};
	
} // namespace VE
