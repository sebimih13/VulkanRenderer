#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <string>

namespace VE
{

	class VEWindow
	{
	public:
		VEWindow(const int& width, const int& height, const std::string& name);
		~VEWindow();

		// Not copyable
		VEWindow(const VEWindow&) = delete;
		VEWindow& operator = (const VEWindow&) = delete;

		inline bool shouldClose() { return glfwWindowShouldClose(window); }
		VkExtent2D getExtent() { return VkExtent2D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();

		GLFWwindow* window;
		
		const int width;
		const int height;

		std::string windowName;
	};

} // namespace VE
