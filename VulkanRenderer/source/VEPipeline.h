#pragma once

#include "VEDevice.h"

#include <string>
#include <vector>

namespace VE
{

	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;

		// Not copyable
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator = (PipelineConfigInfo&) = delete;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class VEPipeline
	{
	public:
		VEPipeline(VEDevice& device, const PipelineConfigInfo& configInfo, const std::string& vertFilePath, const std::string& fragFilePath);
		~VEPipeline();

		// Not copyable
		VEPipeline(const VEPipeline&) = delete;
		void operator = (const VEPipeline&) = delete;

		void Bind(VkCommandBuffer commandBuffer);

		static PipelineConfigInfo& defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(const PipelineConfigInfo& configInfo, const std::string& vertFilePath, const std::string& fragFilePath);

		VEDevice& veDevice;
		VkPipeline graphicsPipeline;

		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	};

} // namespace VE
