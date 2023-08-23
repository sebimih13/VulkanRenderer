#include "VulkanPipeline.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

namespace VulkanRenderer
{

	VulkanPipeline::VulkanPipeline(VulkanDevice& Device, const PipelineConfigInfo& ConfigInfo, const std::string& VertFilePath, const std::string& FragFilePath)
		: Device(Device)
	{
		CreateGraphicsPipeline(ConfigInfo, VertFilePath, FragFilePath);
	}

	VulkanPipeline::~VulkanPipeline()
	{

	}

	PipelineConfigInfo VulkanPipeline::DefaultPipelineConfigInfo(uint32_t Width, uint32_t Height)
	{
		PipelineConfigInfo ConfigInfo;

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
		std::vector<char> VertCode = ReadFile(VertFilePath);
		std::vector<char> FragCode = ReadFile(FragFilePath);

		std::cout << "Vertex Shader Code Size : " << VertCode.size() << '\n';
		std::cout << "Fragment Shader Code Size : " << FragCode.size() << '\n';
	}

	void VulkanPipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* ShaderModule)
	{
		VkShaderModuleCreateInfo CreateInfo;
		CreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		CreateInfo.codeSize = code.size();
		CreateInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		if (vkCreateShaderModule(Device.device(), &CreateInfo, nullptr, ShaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create shader module");
		}
	}

} // namespace VulkanRenderer
