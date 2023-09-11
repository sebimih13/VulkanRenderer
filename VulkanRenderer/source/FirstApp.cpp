#include "FirstApp.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>
#include <array>
#include <iostream>

namespace VE
{

	struct SimplePushConstantData
	{
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	FirstApp::FirstApp()
		: veWindow(WIDTH, HEIGHT, "Vulkan Renderer")
		, veDevice(veWindow)
	{
		loadModels();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}

	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(veDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run()
	{
		while (!veWindow.shouldClose())
		{
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(veDevice.device());
	}

	void FirstApp::loadModels()
	{
		// TODO : rewrite
		std::vector<VEModel::Vertex> vertices = {
			{ { 0.0, -0.5 }, { 1.0f, 0.0f, 0.0f } },
			{ { -0.5, 0.5 }, { 0.0f, 0.0f, 1.0f } },
			{ { 0.5, 0.5 }, { 0.0f, 1.0f, 0.0f } }
		};

		veModel = std::make_unique<VEModel>(veDevice, vertices);
	}

	void FirstApp::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRage = {};
		pushConstantRage.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRage.offset = 0;
		pushConstantRage.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRage;
		if (vkCreatePipelineLayout(veDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void FirstApp::createPipeline()
	{
		assert(veSwapChain != nullptr && "Cannot create pipeline before swap chain");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo& pipelineConfig = VEPipeline::defaultPipelineConfigInfo();
		pipelineConfig.renderPass = veSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vePipeline = std::make_unique<VEPipeline>(veDevice, pipelineConfig, "shaders/push_constants_shader.vert.spv", "shaders/push_constants_shader.frag.spv");
	}

	void FirstApp::createCommandBuffers()
	{
		commandBuffers.resize(veSwapChain->imageCount());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = veDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(veDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers");
		}
	}

	void FirstApp::freeCommandBuffers()
	{
		vkFreeCommandBuffers(veDevice.device(), veDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	void FirstApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = veSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image");
		}

		recordCommandBuffer(imageIndex);
		result = veSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || veWindow.wasWindowResized())
		{
			veWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image");
		}
	}

	void FirstApp::recreateSwapChain()
	{
		VkExtent2D extent = veWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = veWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(veDevice.device());

		if (veSwapChain == nullptr)
		{
			veSwapChain = std::make_unique<VESwapChain>(veDevice, extent);
		}
		else
		{
			veSwapChain = std::make_unique<VESwapChain>(veDevice, extent, std::move(veSwapChain));

			if (veSwapChain->imageCount() != commandBuffers.size())
			{
				freeCommandBuffers();
				createCommandBuffers();
			}
		}

		// check if render pass is compatible before creating the new pipeline
		createPipeline();
	}

	void FirstApp::recordCommandBuffer(int imageIndex)
	{
		static int frame = 0;
		frame = (frame + 1) % 1000;

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer");
		}

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = veSwapChain->getRenderPass();
		renderPassInfo.framebuffer = veSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = veSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// set viewport
		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(veSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(veSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);

		// set scissor
		VkRect2D scissor = { { 0, 0 }, veSwapChain->getSwapChainExtent() };
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		vePipeline->Bind(commandBuffers[imageIndex]);
		veModel->bind(commandBuffers[imageIndex]);

		// push constant
		for (int j = 0; j < 4; ++j)
		{
			SimplePushConstantData push = {};
			push.offset = glm::vec2(-0.5f + frame * 0.002f, -0.4f + j * 0.25f);
			push.color = glm::vec3(0.0f, 0.0f, 0.2f + 0.2f * j);

			vkCmdPushConstants(commandBuffers[imageIndex], pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			veModel->draw(commandBuffers[imageIndex]);
		}

		vkCmdEndRenderPass(commandBuffers[imageIndex]);

		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer");
		}
	}

} // namespace VE
