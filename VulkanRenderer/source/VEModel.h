#pragma once

#include "VEDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace VE
{

	class VEModel
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 normal;
			glm::vec2 uv;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator == (const Vertex& other) const { return position == other.position && color == other.color && normal == other.normal && uv == other.uv; }
		};

		struct Data
		{
			std::vector<Vertex> vertices = {};
			std::vector<uint32_t> indices = {};

			void loadModel(const std::string& filePath);
		};

		VEModel(VEDevice& device, const Data& builder);
		~VEModel();

		/** Not copyable */
		VEModel(const VEModel&) = delete;
		VEModel& operator = (const VEModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

		static std::unique_ptr<VEModel> createModelFromFile(VEDevice& device, const std::string& filePath);

	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
		void createIndexBuffer(const std::vector<uint32_t>& indices);

		VEDevice& veDevice;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

		bool hasIndexBuffer;
		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		uint32_t indexCount;
	};

} // namespace VE
