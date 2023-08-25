#include "VulkanPipeline.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cassert>

namespace VulkanRenderer
{

	VulkanPipeline::VulkanPipeline(VulkanDevice& Device, const PipelineConfigInfo& ConfigInfo, const std::string& VertFilePath, const std::string& FragFilePath)
		: Device(Device)
	{
		CreateGraphicsPipeline(ConfigInfo, VertFilePath, FragFilePath);
	}

	VulkanPipeline::~VulkanPipeline()
	{
		vkDestroyShaderModule(Device.device(), VertShaderModule, nullptr);
		vkDestroyShaderModule(Device.device(), FragShaderModule, nullptr);
		vkDestroyPipeline(Device.device(), GraphicsPipeline, nullptr);
	}

	PipelineConfigInfo VulkanPipeline::DefaultPipelineConfigInfo(uint32_t Width, uint32_t Height)
	{
		PipelineConfigInfo ConfigInfo = {};

		ConfigInfo.Viewport.x = 0.0f;
		ConfigInfo.Viewport.y = 0.0f;
		ConfigInfo.Viewport.width = static_cast<float>(Width);
		ConfigInfo.Viewport.height = static_cast<float>(Height);
		ConfigInfo.Viewport.minDepth = 0.0f;
		ConfigInfo.Viewport.maxDepth = 1.0f;

		ConfigInfo.Scissor.offset = { 0, 0 };
		ConfigInfo.Scissor.extent = { Width, Height };

		ConfigInfo.InputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		ConfigInfo.InputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		ConfigInfo.InputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		ConfigInfo.RasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		ConfigInfo.RasterizationInfo.depthClampEnable = VK_FALSE;
		ConfigInfo.RasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		ConfigInfo.RasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		ConfigInfo.RasterizationInfo.lineWidth = 1.0f;
		ConfigInfo.RasterizationInfo.cullMode = VK_CULL_MODE_NONE;
		ConfigInfo.RasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		ConfigInfo.RasterizationInfo.depthBiasEnable = VK_FALSE;
		ConfigInfo.RasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
		ConfigInfo.RasterizationInfo.depthBiasClamp = 0.0f;           // Optional
		ConfigInfo.RasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional

		ConfigInfo.MultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		ConfigInfo.MultisampleInfo.sampleShadingEnable = VK_FALSE;
		ConfigInfo.MultisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		ConfigInfo.MultisampleInfo.minSampleShading = 1.0f;           // Optional
		ConfigInfo.MultisampleInfo.pSampleMask = nullptr;             // Optional
		ConfigInfo.MultisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
		ConfigInfo.MultisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

		ConfigInfo.ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		ConfigInfo.ColorBlendAttachment.blendEnable = VK_FALSE;
		ConfigInfo.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		ConfigInfo.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		ConfigInfo.ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
		ConfigInfo.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
		ConfigInfo.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
		ConfigInfo.ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

		ConfigInfo.ColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		ConfigInfo.ColorBlendInfo.logicOpEnable = VK_FALSE;
		ConfigInfo.ColorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
		ConfigInfo.ColorBlendInfo.attachmentCount = 1;
		ConfigInfo.ColorBlendInfo.pAttachments = &ConfigInfo.ColorBlendAttachment;
		ConfigInfo.ColorBlendInfo.blendConstants[0] = 0.0f;  // Optional
		ConfigInfo.ColorBlendInfo.blendConstants[1] = 0.0f;  // Optional
		ConfigInfo.ColorBlendInfo.blendConstants[2] = 0.0f;  // Optional
		ConfigInfo.ColorBlendInfo.blendConstants[3] = 0.0f;  // Optional

		ConfigInfo.DepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		ConfigInfo.DepthStencilInfo.depthTestEnable = VK_TRUE;
		ConfigInfo.DepthStencilInfo.depthWriteEnable = VK_TRUE;
		ConfigInfo.DepthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		ConfigInfo.DepthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		ConfigInfo.DepthStencilInfo.minDepthBounds = 0.0f;  // Optional
		ConfigInfo.DepthStencilInfo.maxDepthBounds = 1.0f;  // Optional
		ConfigInfo.DepthStencilInfo.stencilTestEnable = VK_FALSE;
		ConfigInfo.DepthStencilInfo.front = {};  // Optional
		ConfigInfo.DepthStencilInfo.back = {};   // Optional

		return ConfigInfo;
	}

	std::vector<char> VulkanPipeline::ReadFile(const std::string& FilePath)
	{
		std::ifstream File(FilePath, std::ios::ate | std::ios::binary);
		if (!File.is_open())
		{
			throw std::runtime_error("Failed to open file: " + FilePath);
		}

		size_t FileSize = static_cast<size_t>(File.tellg());
		std::vector<char> Buffer(FileSize);

		File.seekg(0);
		File.read(Buffer.data(), FileSize);

		File.close();
		return Buffer;
	}

	void VulkanPipeline::CreateGraphicsPipeline(const PipelineConfigInfo& ConfigInfo, const std::string& VertFilePath, const std::string& FragFilePath)
	{
		assert(ConfigInfo.PipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline: no PipelineLayout provided in ConfigInfo");
		assert(ConfigInfo.RenderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline: no PipelineLayout provided in ConfigInfo");

		std::vector<char> VertCode = ReadFile(VertFilePath);
		std::vector<char> FragCode = ReadFile(FragFilePath);

		CreateShaderModule(VertCode, &VertShaderModule);
		CreateShaderModule(FragCode, &FragShaderModule);

		VkPipelineShaderStageCreateInfo ShadersStages[2];

		// Vertex Shader
		ShadersStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		ShadersStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		ShadersStages[0].module = VertShaderModule;
		ShadersStages[0].pName = "main";
		ShadersStages[0].flags = 0;
		ShadersStages[0].pNext = nullptr;
		ShadersStages[0].pSpecializationInfo = nullptr;

		// Fragment Shader
		ShadersStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		ShadersStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		ShadersStages[1].module = FragShaderModule;
		ShadersStages[1].pName = "main";
		ShadersStages[1].flags = 0;
		ShadersStages[1].pNext = nullptr;
		ShadersStages[1].pSpecializationInfo = nullptr;

		VkPipelineVertexInputStateCreateInfo VertexInputInfo = {};
		VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		VertexInputInfo.vertexAttributeDescriptionCount = 0;
		VertexInputInfo.vertexBindingDescriptionCount = 0;
		VertexInputInfo.pVertexAttributeDescriptions = nullptr;
		VertexInputInfo.pVertexBindingDescriptions = nullptr;

		VkPipelineViewportStateCreateInfo ViewportInfo = {};
		ViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		ViewportInfo.viewportCount = 1;
		ViewportInfo.pViewports = &ConfigInfo.Viewport;
		ViewportInfo.scissorCount = 1;
		ViewportInfo.pScissors = &ConfigInfo.Scissor;

		VkGraphicsPipelineCreateInfo PipelineInfo = {};
		PipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		PipelineInfo.stageCount = 2;
		PipelineInfo.pStages = ShadersStages;
		PipelineInfo.pVertexInputState = &VertexInputInfo;
		PipelineInfo.pInputAssemblyState = &ConfigInfo.InputAssemblyInfo;
		PipelineInfo.pViewportState = &ViewportInfo;
		PipelineInfo.pRasterizationState = &ConfigInfo.RasterizationInfo;
		PipelineInfo.pMultisampleState = &ConfigInfo.MultisampleInfo;
		PipelineInfo.pColorBlendState = &ConfigInfo.ColorBlendInfo;
		PipelineInfo.pDepthStencilState = &ConfigInfo.DepthStencilInfo;
		PipelineInfo.pDynamicState = nullptr;
		PipelineInfo.layout = ConfigInfo.PipelineLayout;
		PipelineInfo.renderPass = ConfigInfo.RenderPass;
		PipelineInfo.subpass = ConfigInfo.Subpass;
		PipelineInfo.basePipelineIndex = -1;
		PipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(Device.device(), VK_NULL_HANDLE, 1, &PipelineInfo, nullptr, &GraphicsPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline");
		}
	}

	void VulkanPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* ShaderModule)
	{
		VkShaderModuleCreateInfo CreateInfo = {};
		CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		CreateInfo.codeSize = code.size();
		CreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(Device.device(), &CreateInfo, nullptr, ShaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}
	}

} // namespace VulkanRenderer
