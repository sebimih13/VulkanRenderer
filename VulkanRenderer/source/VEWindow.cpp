#include "VEWindow.h"

#include <stdexcept>

namespace VE
{

	VEWindow::VEWindow(const int& width, const int& height, const std::string& name)
		: width(width)
		, height(height)
		, framebufferResized(false)
		, windowName(name)
	{
		initWindow();
	}

	VEWindow::~VEWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void VEWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void VEWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}

	void VEWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		VEWindow* veWindow = reinterpret_cast<VEWindow*>(glfwGetWindowUserPointer(window));
		veWindow->framebufferResized = true;
		veWindow->width = width;
		veWindow->height = height;
	}

} // namespace VE
