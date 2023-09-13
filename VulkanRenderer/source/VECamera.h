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

		inline const glm::mat4& getProjection() const { return projectionMatrix; }

	private:
		glm::mat4 projectionMatrix;
	};

} // namespace VE
