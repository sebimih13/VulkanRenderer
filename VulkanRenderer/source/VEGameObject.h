#pragma once

#include "VEModel.h"

#include <memory>
#include <unordered_map>

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
		glm::mat3 normalMatrix();
	};

	struct PointLightComponent
	{
		float lightIntensity = 1.0f;
	};

	class VEGameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, VEGameObject>;

		/** not copyable */
		VEGameObject(const VEGameObject&) = delete;
		VEGameObject& operator = (const VEGameObject&) = delete;

		/** movable */
		VEGameObject(VEGameObject&&) = default;
		VEGameObject& operator = (VEGameObject&&) = default;

		static VEGameObject createGameObject();
		static VEGameObject makePointLight(float intensity = 10.0f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.0f));

		inline const id_t getID() const { return id; }

		glm::vec3 color;

		TransformComponent transform = {};

		// optional pointer components
		std::shared_ptr<VEModel> model;
		std::unique_ptr<PointLightComponent> pointLight;

	private:
		VEGameObject(id_t objID);

		id_t id;
	};

} // namespace VE
