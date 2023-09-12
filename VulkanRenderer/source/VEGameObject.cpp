#include "VEGameObject.h"

#include <glm/gtc/matrix_transform.hpp>

namespace VE
{

	/* ************************************************************************************************************************ */
	/* ***    TransformComponent    ***************************************************************************************** */
	/* ************************************************************************************************************************ */

	/* Unoptimized mat4()

		glm::mat4 TransformComponent::mat4()
		{
			// apply translation
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);

			// apply rotation
			transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			// apply scaling
			transform = glm::scale(transform, scale);

			return transform;
		}

	*/

	// optimized mat4()
	glm::mat4 TransformComponent::mat4() 
	{
		const float c3 = glm::cos(rotation.z);
		const float s3 = glm::sin(rotation.z);
		const float c2 = glm::cos(rotation.x);
		const float s2 = glm::sin(rotation.x);
		const float c1 = glm::cos(rotation.y);
		const float s1 = glm::sin(rotation.y);

		return glm::mat4(
			glm::vec4(scale.x * (c1 * c3 + s1 * s2 * s3), scale.x * (c2 * s3), scale.x * (c1 * s2 * s3 - c3 * s1), 0.0f),
			glm::vec4(scale.y * (c3 * s1 * s2 - c1 * s3), scale.y * (c2 * c3), scale.y * (c1 * c3 * s2 + s1 * s3), 0.0f),
			glm::vec4(scale.z * (c2 * s1), scale.z * (-s2), scale.z * (c1 * c2), 0.0f),
			glm::vec4(translation.x, translation.y, translation.z, 1.0f)
		);
	}

	/* ************************************************************************************************************************ */
	/* ***    VEGameObject    ************************************************************************************************* */
	/* ************************************************************************************************************************ */
	VEGameObject::VEGameObject(id_t objID)
		: id(objID)
	{

	}

	VEGameObject VEGameObject::createGameObject()
	{
		static id_t currentID = 0;
		return VEGameObject(currentID++);
	}

} // namespace VE
