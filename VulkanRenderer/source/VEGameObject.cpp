#include "VEGameObject.h"

namespace VE
{

	/* ************************************************************************************************************************ */
	/* ***    Transform2DComponent    ***************************************************************************************** */
	/* ************************************************************************************************************************ */
	glm::mat2 Transform2DComponent::mat2()
	{
		// rotation matrix
		const float s = glm::sin(rotation);
		const float c = glm::cos(rotation);
		glm::mat2 rotationMatrix = glm::mat2(glm::vec2(c, s), glm::vec2(-s, c));

		// scale matrix
		glm::mat2 scaleMatrix = glm::mat2(glm::vec2(scale.x, 0.0f), glm::vec2(0.0f, scale.y));

		return rotationMatrix * scaleMatrix;
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
