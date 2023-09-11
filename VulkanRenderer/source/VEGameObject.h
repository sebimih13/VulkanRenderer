#pragma once

#include "VEModel.h"

#include <memory>

namespace VE
{

	struct Transform2DComponent
	{
		glm::vec2 translation = {};		// position offset
		glm::vec2 scale = glm::vec2(1.0f, 1.0f);
		float rotation;

		glm::mat2 mat2();
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

		Transform2DComponent transform2D;

	private:
		VEGameObject(id_t objID);

		id_t id;
	};

} // namespace VE
