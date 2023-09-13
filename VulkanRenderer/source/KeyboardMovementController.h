#pragma once

#include "VEGameObject.h"
#include "VEWindow.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace VE
{

	class KeyboardMovementController
	{
	public:
		struct KeyMappings
		{
			int moveLeft = GLFW_KEY_A;
			int moveRight = GLFW_KEY_D;
			int moveForward = GLFW_KEY_W;
			int moveBackward = GLFW_KEY_S;
			int moveUp = GLFW_KEY_E;
			int moveDown = GLFW_KEY_Q;
			int lookLeft = GLFW_KEY_LEFT;
			int lookRight = GLFW_KEY_RIGHT;
			int lookUp = GLFW_KEY_UP;
			int lookDown = GLFW_KEY_DOWN;
		};

		KeyboardMovementController();
		~KeyboardMovementController();

		void moveInPlaneXZ(GLFWwindow* window, float deltaTime, VEGameObject& gameObject);

	private:
		KeyMappings keys;

		float moveSpeed;
		float lookSpeed;
	};

} // namespace VE
