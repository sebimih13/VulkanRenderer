#include "FirstApp.h"
#include "VERenderSystem.h"
#include "VECamera.h"
#include "KeyboardMovementController.h"
#include "VEBuffer.h"
#include "systems/PointLightSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <array>
#include <chrono>

namespace VE
{

	FirstApp::FirstApp()
		: veWindow(WIDTH, HEIGHT, "Vulkan Renderer")
		, veDevice(veWindow)
		, veRenderer(veWindow, veDevice)
	{
		globalPool = VEDescriptorPool::Builder(veDevice)
			.setMaxSets(VESwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VESwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VESwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();

		loadGameObjects();
	}

	FirstApp::~FirstApp()
	{
		
	}

	void FirstApp::run()
	{
		std::vector<std::unique_ptr<VEBuffer>> uboBuffers(VESwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < uboBuffers.size(); ++i)
		{
			uboBuffers[i] = std::make_unique<VEBuffer>(
				veDevice,
				sizeof(GlobalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			);

			uboBuffers[i]->map();
		}

		auto globalSetLayout = VEDescriptorSetLayout::Builder(veDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

		std::vector<VkDescriptorSet> globalDescriptorSets(VESwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); ++i)
		{
			VkDescriptorBufferInfo bufferInfo = uboBuffers[i]->descriptorInfo();
			VEDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		RenderSystem renderSystem(veDevice, veRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());
		PointLightSystem pointLightSystem(veDevice, veRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout());

		VECamera camera;
		camera.setViewTarget(glm::vec3(-1.0f, -2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

		VEGameObject viewerObject = VEGameObject::createGameObject();
		viewerObject.transform.translation.z = -2.5f;

		KeyboardMovementController cameraController;

		// time
		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!veWindow.shouldClose())
		{
			glfwPollEvents();
			
			// time
			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			// input
			cameraController.moveInPlaneXZ(veWindow.getGLFWWindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			// setup camera
			float aspect = veRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 1000.0f);
			
			// draw
			if (VkCommandBuffer commandBuffer = veRenderer.beginFrame())
			{
				// create frame info
				int frameIndex = veRenderer.getFrameIndex();
				FrameInfo frameInfo = {
					frameIndex,
					frameTime,
					commandBuffer,
					camera,
					globalDescriptorSets[frameIndex],
					gameObjects
				};

				// update
				GlobalUBO ubo = {};
				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();
				ubo.inverseView = camera.getInverseView();

				pointLightSystem.update(frameInfo, ubo);

				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				veRenderer.beginSwapChainRenderPass(commandBuffer);
				
				// order here matters
				renderSystem.renderGameObjects(frameInfo);
				pointLightSystem.render(frameInfo);
				
				veRenderer.endSwapChainRenderPass(commandBuffer);
				veRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(veDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		// flat vase
		VEGameObject flatVase = VEGameObject::createGameObject();
		flatVase.model = VEModel::createModelFromFile(veDevice, "models/flat_vase.obj");
		flatVase.transform.translation = glm::vec3(-0.5f, 0.5f, 0.0f);
		flatVase.transform.scale = glm::vec3(3.0f, 1.5f, 3.0f);

		gameObjects.emplace(flatVase.getID(), std::move(flatVase));

		// smooth vase
		VEGameObject smoothVase = VEGameObject::createGameObject();
		smoothVase.model = VEModel::createModelFromFile(veDevice, "models/smooth_vase.obj");
		smoothVase.transform.translation = glm::vec3(0.5f, 0.5f, 0.0f);
		smoothVase.transform.scale = glm::vec3(3.0f, 1.5f, 3.0f);

		gameObjects.emplace(smoothVase.getID(), std::move(smoothVase));

		// quad
		VEGameObject floor = VEGameObject::createGameObject();
		floor.model = VEModel::createModelFromFile(veDevice, "models/quad.obj");
		floor.transform.translation = glm::vec3(0.0f, 0.5f, 0.0f);
		floor.transform.scale = glm::vec3(3.0f, 1.0f, 3.0f);

		gameObjects.emplace(floor.getID(), std::move(floor));

		// point lights
		std::vector<glm::vec3> lightColors = {
			 { 1.0f, 0.1f, 0.1f },
			 { 0.1f, 0.1f, 1.f },
			 { 0.1f, 1.0f, 0.1f },
			 { 1.0f, 1.0f, 0.1f },
			 { 0.1f, 1.0f, 1.0f },
			 { 1.0f, 1.0f, 1.0f }
		};

		for (int i = 0; i < lightColors.size(); i++)
		{
			VEGameObject pointLight = VEGameObject::makePointLight(0.2f);
			pointLight.color = lightColors[i];
			glm::mat4 rotateLight = glm::rotate(glm::mat4(1.0f), (i * glm::two_pi<float>()) / lightColors.size(), glm::vec3(0.0f, -1.0f, 0.0f));
			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.0f, -1.0f, -1.0f, -1.0f));

			gameObjects.emplace(pointLight.getID(), std::move(pointLight));

		}
	}

} // namespace VE
