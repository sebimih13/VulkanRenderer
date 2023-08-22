#include "VulkanPipeline.h"

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <filesystem>	// TODO : delete

namespace VulkanRenderer
{

	VulkanPipeline::VulkanPipeline(const std::string& VertFilePath, const std::string& FragFilePath)
	{
		CreateGraphicsPipeline(VertFilePath, FragFilePath);
	}

	VulkanPipeline::~VulkanPipeline()
	{

	}

	std::vector<char> VulkanPipeline::ReadFile(const std::string& FilePath)
	{
		std::fstream File(FilePath, std::ios::ate | std::ios::binary);
		if (!File.is_open())
		{
			std::cout << std::filesystem::current_path() << '\n';
			std::cout << FilePath << '\n';
			throw std::runtime_error("Failed to open file: " + FilePath);
		}

		size_t FileSize = static_cast<size_t>(File.tellg());
		std::vector<char> Buffer(FileSize);

		File.seekg(0);
		File.read(Buffer.data(), FileSize);

		File.close();
		return Buffer;
	}

	void VulkanPipeline::CreateGraphicsPipeline(const std::string& VertFilePath, const std::string& FragFilePath)
	{
		std::vector<char> VertCode = ReadFile(VertFilePath);
		std::vector<char> FragCode = ReadFile(FragFilePath);

		std::cout << "Vertex Shader Code Size : " << VertCode.size() << '\n';
		std::cout << "Fragment Shader Code Size : " << FragCode.size() << '\n';
	}

} // namespace VulkanRenderer
