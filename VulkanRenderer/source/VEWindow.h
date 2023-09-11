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

		/** Not copyable */
		VEWindow(const VEWindow&) = delete;
		VEWindow& operator = (const VEWindow&) = delete;

		inline bool shouldClose() { return glfwWindowShouldClose(window); }
		inline VkExtent2D getExtent() { return VkExtent2D{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
		inline bool wasWindowResized() { return framebufferResized; }
		inline void resetWindowResizedFlag() { framebufferResized = false; }

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private:
		void initWindow();

		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

		GLFWwindow* window;
		
		int width;
		int height;
		bool framebufferResized;

		std::string windowName;
	};

} // namespace VE
