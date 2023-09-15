#pragma once

#include "VECamera.h"
#include "VEGameObject.h"

#include <vulkan/vulkan.h>

namespace VE
{

#define MAX_LIGHTS 10

	struct PointLight
	{
		glm::vec4 position;
		glm::vec4 color;
	};

	struct GlobalUBO
	{
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 inverseView = glm::mat4(1.0f);
		glm::vec4 ambientLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.02f);
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

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
