#include "VulkanWindow.h"

#include <stdexcept>

namespace VulkanRenderer
{

	VulkanWindow::VulkanWindow(const int& Width, const int& Height, const std::string& Name)
		: Width(Width)
		, Height(Height)
		, WindowName(Name)
	{
		InitializeWindow();
	}

	VulkanWindow::~VulkanWindow()
	{
		glfwDestroyWindow(Window);
		glfwTerminate();
	}

	void VulkanWindow::createWindowSurface(VkInstance Instance, VkSurfaceKHR* Surface)
	{
		if (glfwCreateWindowSurface(Instance, Window, nullptr, Surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void VulkanWindow::InitializeWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		Window = glfwCreateWindow(Width, Height, WindowName.c_str(), nullptr, nullptr);
	}

} // namespace VulkanWindow
