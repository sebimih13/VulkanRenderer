#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <string>

namespace VulkanRenderer
{

	class VulkanWindow
	{
	public:
		VulkanWindow(const int& Width, const int& Height, const std::string& Name);
		virtual ~VulkanWindow();

		VulkanWindow(const VulkanWindow&) = delete;
		VulkanWindow& operator = (const VulkanWindow&) = delete;

		inline bool ShouldClose() { return glfwWindowShouldClose(Window); }
		VkExtent2D GetExtent() { return VkExtent2D{ static_cast<uint32_t>(Width), static_cast<uint32_t>(Height) }; }

		void createWindowSurface(VkInstance Instance, VkSurfaceKHR* Surface);

	private:
		void InitializeWindow();

		GLFWwindow* Window;
		
		const int Width;
		const int Height;

		std::string WindowName;
	};

} // namespace VulkanRenderer
