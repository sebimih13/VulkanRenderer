#include "FirstApp.h"
#include "VERenderSystem.h"
#include "VECamera.h"
#include "KeyboardMovementController.h"

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
		RenderSystem renderSystem(veDevice, veRenderer.getSwapChainRenderPass());
		VECamera camera;
		//camera.setViewDirection(glm::vec3(0.0f), glm::vec3(0.5f, 0.0f, 1.0f));
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
			float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			// input
			cameraController.moveInPlaneXZ(veWindow.getGLFWWindow(), deltaTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			// setup camera
			float aspect = veRenderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);
			
			// draw
			if (VkCommandBuffer commandBuffer = veRenderer.beginFrame())
			{
				veRenderer.beginSwapChainRenderPass(commandBuffer);
				
				renderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				
				veRenderer.endSwapChainRenderPass(commandBuffer);
				veRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(veDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<VEModel> veModel = VEModel::createModelFromFile(veDevice, "models/smooth_vase.obj");

		VEGameObject gameobj = VEGameObject::createGameObject();
		gameobj.model = veModel;
		gameobj.transform.translation = glm::vec3(0.0f, 0.0f, 2.5f);
		gameobj.transform.scale = glm::vec3(3.0f);

		gameObjects.push_back(std::move(gameobj));
	}

} // namespace VE
