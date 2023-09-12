#include "FirstApp.h"
#include "RenderSystem.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <stdexcept>
#include <array>
#include <iostream>

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

		while (!veWindow.shouldClose())
		{
			glfwPollEvents();
			
			if (VkCommandBuffer commandBuffer = veRenderer.beginFrame())
			{
				veRenderer.beginSwapChainRenderPass(commandBuffer);
				
				renderSystem.renderGameObjects(commandBuffer, gameObjects);
				
				veRenderer.endSwapChainRenderPass(commandBuffer);
				veRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(veDevice.device());
	}

	void FirstApp::loadGameObjects()
	{
		// TODO : rewrite
		std::vector<VEModel::Vertex> vertices = {
			{ { 0.0, -0.5 }, { 1.0f, 0.0f, 0.0f } },
			{ { -0.5, 0.5 }, { 0.0f, 0.0f, 1.0f } },
			{ { 0.5, 0.5 }, { 0.0f, 1.0f, 0.0f } }
		};

		std::shared_ptr<VEModel> veModel = std::make_shared<VEModel>(veDevice, vertices);

		auto triangle = VEGameObject::createGameObject();
		triangle.model = veModel;
		triangle.color = glm::vec3(0.1f, 0.8f, 0.1f);
		triangle.transform2D.translation.x = 0.2f;
		triangle.transform2D.translation.y = 0.0f;
		triangle.transform2D.scale = glm::vec2(2.0f, 0.5f);
		triangle.transform2D.rotation = 0.25f * glm::two_pi<float>();

		gameObjects.push_back(std::move(triangle));
	}

} // namespace VE
