#pragma once

#include "VEWindow.h"
#include "VEDevice.h"
#include "VEGameObject.h"
#include "VERenderer.h"

#include <memory>
#include <vector>

namespace VE
{

	class FirstApp
	{
	public:
		FirstApp();
		~FirstApp();

		/** Not copyable */
		FirstApp(const FirstApp&) = delete;
		FirstApp& operator = (const FirstApp&) = delete;

		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		void run();

	private:
		void loadGameObjects();

		VEWindow veWindow;
		VEDevice veDevice;
		VERenderer veRenderer;

		std::vector<VEGameObject> gameObjects;
	};

} // namespace VE
