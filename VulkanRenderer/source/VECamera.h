#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace VE
{

	class VECamera
	{
	public:
		VECamera();
		~VECamera();

		void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
		void setPerspectiveProjection(float fovy, float aspect, float near, float far);

		void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));
		void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));
		void setViewYXZ(glm::vec3 position, glm::vec3 rotation);

		inline const glm::mat4& getProjection() const { return projectionMatrix; }
		inline const glm::mat4& getView() const { return viewMatrix; }

	private:
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
	};

} // namespace VE