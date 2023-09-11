#include "SierpinskiTriangleApp.h"

#include <stdexcept>
#include <array>

// TODO : implement App interface

namespace VE
{
	
	std::vector<VEModel::Vertex> generateVertices(const std::vector<VEModel::Vertex>& vertices, int step = 1)
	{
		if (step == 7)
		{
			return vertices;
		}

		/*
			Vertex[i] = top-up
			Vertex[i + 1] = left-bottom
			Vertex[i + 2] = right-bottom
		*/
		std::vector<VEModel::Vertex> newVertices;
		for (int i = 0; i < vertices.size(); i += 3)
		{
			// calculate the empty triangle inside
			float midBottomVertexPosX = (vertices[i + 1].position.x + vertices[i + 2].position.x) / 2.0f;
			float midBottomVertexPosY = (vertices[i + 1].position.y + vertices[i + 2].position.y) / 2.0f;
			VEModel::Vertex midBottomVertex = { glm::vec2(midBottomVertexPosX, midBottomVertexPosY) };
			
			float midLeftVertexPosX = (vertices[i].position.x + vertices[i + 1].position.x) / 2.0f;
			float midLeftVertexPosY = (vertices[i].position.y + vertices[i + 1].position.y) / 2.0f;
			VEModel::Vertex midLeftVertex = { glm::vec2(midLeftVertexPosX, midLeftVertexPosY) };
			
			float midRightVertexPosX = (vertices[i].position.x + vertices[i + 2].position.x) / 2.0f;
			float midRightVertexPosY = (vertices[i].position.y + vertices[i + 2].position.y) / 2.0f;
			VEModel::Vertex midRightVertex = { glm::vec2(midRightVertexPosX, midRightVertexPosY) };

			// add the vertices for the new triangles in order

			// calculate the new top triangle
			newVertices.push_back(vertices[i]);		// top
			newVertices.push_back(midLeftVertex);	// left-bottom
			newVertices.push_back(midRightVertex);	// right-bottom
			
			// calculate the new left triangle
			newVertices.push_back(midLeftVertex);	// top
			newVertices.push_back(vertices[i + 1]);	// left-bottom
			newVertices.push_back(midBottomVertex);	// right-bottom
			
			// calculate the new right triangle
			newVertices.push_back(midRightVertex);	// top
			newVertices.push_back(midBottomVertex);	// left-bottom
			newVertices.push_back(vertices[i + 2]);	// right-bottom
		}

		return generateVertices(newVertices, step + 1);
	}

	SierpinskiTriangleApp::SierpinskiTriangleApp()
		: veWindow(WIDTH, HEIGHT, "Vulkan Renderer")
		, veDevice(veWindow)
		, veSwapChain(veDevice, veWindow.getExtent())
	{
		loadModels();
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	SierpinskiTriangleApp::~SierpinskiTriangleApp()
	{
		vkDestroyPipelineLayout(veDevice.device(), pipelineLayout, nullptr);
	}

	void SierpinskiTriangleApp::run()
	{
		while (!veWindow.shouldClose())
		{
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(veDevice.device());
	}

	void SierpinskiTriangleApp::loadModels()
	{
		// TODO : rewrite
		std::vector<VEModel::Vertex> vertices = {
			{{ 0.0, -0.9 }},
			{{ -0.9, 0.9 }},
			{{ 0.9, 0.9 }}
		};

		vertices = generateVertices(vertices);

		veModel = std::make_unique<VEModel>(veDevice, vertices);
	}

	void SierpinskiTriangleApp::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(veDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}

	void SierpinskiTriangleApp::createPipeline()
	{
		PipelineConfigInfo& pipelineConfig = VEPipeline::defaultPipelineConfigInfo();
		pipelineConfig.renderPass = veSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		vePipeline = std::make_unique<VEPipeline>(veDevice, pipelineConfig, "shaders/sierpinski_triangle_shader.vert.spv", "shaders/sierpinski_triangle_shader.frag.spv");
	}

	void SierpinskiTriangleApp::createCommandBuffers()
	{
		commandBuffers.resize(veSwapChain.imageCount());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = veDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(veDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to allocate command buffers");
		}

		for (int i = 0; i < commandBuffers.size(); ++i)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to begin recording command buffer");
			}

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = veSwapChain.getRenderPass();
			renderPassInfo.framebuffer = veSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = veSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues = {};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vePipeline->bind(commandBuffers[i]);
			veModel->bind(commandBuffers[i]);
			veModel->draw(commandBuffers[i]);

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Failed to record command buffer");
			}
		}
	}

	void SierpinskiTriangleApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = veSwapChain.acquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image");
		}

		result = veSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image");
		}
	}

} // namespace VE
