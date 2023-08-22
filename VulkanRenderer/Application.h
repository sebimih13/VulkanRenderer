#pragma once

#include "VulkanWindow.h"
#include "VulkanPipeline.h"

namespace VulkanRenderer
{

	class Application
	{
	public:
		/** Constructor */
		Application();
		virtual ~Application();

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void Run();

	private:
		VulkanWindow Window;
		VulkanPipeline Pipeline;
	};

} // namespace VulkanRenderer
