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

	// TODO : temporary helper function, creates a 1x1x1 cube centered at offset
	std::unique_ptr<VEModel> createCubeModel(VEDevice& device, glm::vec3 offset) 
	{
		std::vector<VEModel::Vertex> vertices{

			// left face (white)
			{{-.5f, -.5f, -.5f}, { .9f, .9f, .9f }},
			{ {-.5f, .5f, .5f}, {.9f, .9f, .9f} },
			{ {-.5f, -.5f, .5f}, {.9f, .9f, .9f} },
			{ {-.5f, -.5f, -.5f}, {.9f, .9f, .9f} },
			{ {-.5f, .5f, -.5f}, {.9f, .9f, .9f} },
			{ {-.5f, .5f, .5f}, {.9f, .9f, .9f} },

				// right face (yellow)
			{ {.5f, -.5f, -.5f}, {.8f, .8f, .1f} },
			{ {.5f, .5f, .5f}, {.8f, .8f, .1f} },
			{ {.5f, -.5f, .5f}, {.8f, .8f, .1f} },
			{ {.5f, -.5f, -.5f}, {.8f, .8f, .1f} },
			{ {.5f, .5f, -.5f}, {.8f, .8f, .1f} },
			{ {.5f, .5f, .5f}, {.8f, .8f, .1f} },

				// top face (orange, remember y axis points down)
			{ {-.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
			{ {.5f, -.5f, .5f}, {.9f, .6f, .1f} },
			{ {-.5f, -.5f, .5f}, {.9f, .6f, .1f} },
			{ {-.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
			{ {.5f, -.5f, -.5f}, {.9f, .6f, .1f} },
			{ {.5f, -.5f, .5f}, {.9f, .6f, .1f} },

				// bottom face (red)
			{ {-.5f, .5f, -.5f}, {.8f, .1f, .1f} },
			{ {.5f, .5f, .5f}, {.8f, .1f, .1f} },
			{ {-.5f, .5f, .5f}, {.8f, .1f, .1f} },
			{ {-.5f, .5f, -.5f}, {.8f, .1f, .1f} },
			{ {.5f, .5f, -.5f}, {.8f, .1f, .1f} },
			{ {.5f, .5f, .5f}, {.8f, .1f, .1f} },

				// nose face (blue)
			{ {-.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
			{ {.5f, .5f, 0.5f}, {.1f, .1f, .8f} },
			{ {-.5f, .5f, 0.5f}, {.1f, .1f, .8f} },
			{ {-.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
			{ {.5f, -.5f, 0.5f}, {.1f, .1f, .8f} },
			{ {.5f, .5f, 0.5f}, {.1f, .1f, .8f} },

				// tail face (green)
			{ {-.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
			{ {.5f, .5f, -0.5f}, {.1f, .8f, .1f} },
			{ {-.5f, .5f, -0.5f}, {.1f, .8f, .1f} },
			{ {-.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
			{ {.5f, -.5f, -0.5f}, {.1f, .8f, .1f} },
			{ {.5f, .5f, -0.5f}, {.1f, .8f, .1f} },

		};
		
		for (auto& v : vertices) 
		{
			v.position += offset;
		}

		return std::make_unique<VEModel>(device, vertices);
	}

	void FirstApp::loadGameObjects()
	{
		std::shared_ptr<VEModel> veModel = createCubeModel(veDevice, glm::vec3(0.0f, 0.0f, 0.0f));

		auto cube = VEGameObject::createGameObject();
		cube.model = veModel;
		cube.transform.translation = glm::vec3(0.0f, 0.0f, 2.5f);
		cube.transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);

		gameObjects.push_back(std::move(cube));
	}

} // namespace VE
