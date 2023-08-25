#pragma once

#include "VulkanDevice.h"

#include <string>
#include <vector>

namespace VulkanRenderer
{

	struct PipelineConfigInfo
	{
		VkViewport Viewport;
		VkRect2D Scissor;
		VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo MultisampleInfo;
		VkPipelineColorBlendAttachmentState ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo DepthStencilInfo;
		VkPipelineLayout PipelineLayout = nullptr;
		VkRenderPass RenderPass = nullptr;
		uint32_t Subpass = 0;
	};

	class VulkanPipeline
	{
	public:
		VulkanPipeline(VulkanDevice& Device, const PipelineConfigInfo& ConfigInfo, const std::string& VertFilePath, const std::string& FragFilePath);
		~VulkanPipeline();

		VulkanPipeline(const VulkanPipeline&) = delete;
		void operator=(const VulkanPipeline&) = delete;

		void Bind(VkCommandBuffer CommandBuffer);

		static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t Width, uint32_t Height);

	private:
		static std::vector<char> ReadFile(const std::string& FilePath);

		void CreateGraphicsPipeline(const PipelineConfigInfo& ConfigInfo, const std::string& VertFilePath, const std::string& FragFilePath);

		VulkanDevice& Device;
		VkPipeline GraphicsPipeline;

		VkShaderModule VertShaderModule;
		VkShaderModule FragShaderModule;

		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* ShaderModule);
	};

} // namespace VulkanRenderer
