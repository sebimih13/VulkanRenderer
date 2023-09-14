#include "FirstApp.h"
#include "VERenderSystem.h"
#include "VECamera.h"
#include "KeyboardMovementController.h"
#include "VEBuffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <stdexcept>
#include <array>
#include <chrono>

namespace VE
{

	struct GlobalUBO
	{
		glm::mat4 projectionView = glm::mat4(1.0f);
		glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0f, -3.0f, -1.0f));
	};


	FirstApp::FirstApp()
		: veWindow(WIDTH, HEIGHT, "Vulkan Renderer")
		, veDevice(veWindow)
		, veRenderer(veWindow, veDevice)
	{
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

		RenderSystem renderSystem(veDevice, veRenderer.getSwapChainRenderPass());
		VECamera camera;
		camera.setViewTarget(glm::vec3(-1.0f, -2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

		VEGameObject viewerObject = VEGameObject::createGameObject();
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
			camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);
			
			// draw
			if (VkCommandBuffer commandBuffer = veRenderer.beginFrame())
			{
				// create frame info
				int frameIndex = veRenderer.getFrameIndex();
				FrameInfo frameInfo = {
					frameIndex,
					frameTime,
					commandBuffer,
					camera
				};

				// update
				GlobalUBO ubo = {};
				ubo.projectionView = camera.getProjection() * camera.getView();
				uboBuffers[frameIndex]->writeToBuffer(&ubo);
				uboBuffers[frameIndex]->flush();

				// render
				veRenderer.beginSwapChainRenderPass(commandBuffer);
				
				renderSystem.renderGameObjects(frameInfo, gameObjects);
				
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
		flatVase.transform.translation = glm::vec3(-0.5f, 0.5f, 2.5f);
		flatVase.transform.scale = glm::vec3(3.0f, 1.5f , 3.0f);

		gameObjects.push_back(std::move(flatVase));

		// smooth vase
		VEGameObject smoothVase = VEGameObject::createGameObject();
		smoothVase.model = VEModel::createModelFromFile(veDevice, "models/smooth_vase.obj");
		smoothVase.transform.translation = glm::vec3(0.5f, 0.5f, 2.5f);
		smoothVase.transform.scale = glm::vec3(3.0f, 1.5f , 3.0f);

		gameObjects.push_back(std::move(smoothVase));
	}

} // namespace VE
