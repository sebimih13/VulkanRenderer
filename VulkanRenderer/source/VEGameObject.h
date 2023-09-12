#pragma once

#include "VEModel.h"

#include <memory>

namespace VE
{

	struct TransformComponent
	{
		glm::vec3 translation = {};
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 rotation = {};

		/**	
		*	Matrix corresponds to (translate * rotationY * rotationX * rotationZ * scale) transformation
		*	Rotation convention uses tait-bryan angles with axis order Y(1), X(2), Z(3)
		*/
		glm::mat4 mat4();
	};

	class VEGameObject
	{
	public:
		using id_t = unsigned int;

		/** not copyable */
		VEGameObject(const VEGameObject&) = delete;
		VEGameObject& operator = (const VEGameObject&) = delete;

		/** movable */
		VEGameObject(VEGameObject&&) = default;
		VEGameObject& operator = (VEGameObject&&) = default;

		static VEGameObject createGameObject();

		inline const id_t getID() const { return id; }

		std::shared_ptr<VEModel> model;
		glm::vec3 color;

		TransformComponent transform = {};

	private:
		VEGameObject(id_t objID);

		id_t id;
	};

} // namespace VE
