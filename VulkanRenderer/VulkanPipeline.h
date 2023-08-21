#pragma once

#include <string>
#include <vector>

namespace VulkanRenderer
{

	class VulkanPipeline
	{
	public:
		VulkanPipeline(const std::string& VertFilePath, const std::string& FragFilePath);
		virtual ~VulkanPipeline();

	private:
		static std::vector<char> ReadFile(const std::string& FilePath);

		void CreateGraphicsPipeline(const std::string& VertFilePath, const std::string& FragFilePath);
	};

} // namespace VulkanRenderer