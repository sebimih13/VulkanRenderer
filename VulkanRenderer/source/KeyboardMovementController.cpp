#include "KeyboardMovementController.h"

#include <limits>

namespace VE
{

	KeyboardMovementController::KeyboardMovementController()
		: moveSpeed(3.5f)
		, lookSpeed(1.5f)
	{

	}

	KeyboardMovementController::~KeyboardMovementController()
	{

	}

	void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float deltaTime, VEGameObject& gameObject)
	{
		// calculate rotation
		glm::vec3 rotate(0.0f);

		if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS)
		{
			rotate.y += 1.0f;
		}

		if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS)
		{
			rotate.y -= 1.0f;
		}

		if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS)
		{
			rotate.x += 1.0f;
		}

		if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS)
		{
			rotate.x -= 1.0f;
		}

		if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
		{
			gameObject.transform.rotation += lookSpeed * deltaTime * glm::normalize(rotate);
		}

		// limit pitch values between about +/- 85 degrees
		gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
		gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

		// calculate translation
		float yaw = gameObject.transform.rotation.y;
		const glm::vec3 forwardDir = glm::vec3(glm::sin(yaw), 0.0f, glm::cos(yaw));
		const glm::vec3 rightDir = glm::vec3(forwardDir.z, 0.0f, -forwardDir.x);
		const glm::vec3 upDir = glm::vec3(0.0f, -1.0f, 0.0f);

		glm::vec3 moveDir = glm::vec3(0.0f);

		if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS)
		{
			moveDir += forwardDir;
		}

		if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS)
		{
			moveDir -= forwardDir;
		}

		if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS)
		{
			moveDir += rightDir;
		}

		if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS)
		{
			moveDir -= rightDir;
		}

		if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS)
		{
			moveDir += upDir;
		}

		if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS)
		{
			moveDir -= upDir;
		}

		if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
		{
			gameObject.transform.translation += moveSpeed * deltaTime * glm::normalize(moveDir);
		}
	}

} // namespace VE
