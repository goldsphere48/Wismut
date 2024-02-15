#pragma once
#include <glm/glm.hpp>

namespace Wi
{
	enum class DataFormat
	{
		Float,
		Vec2,
		Vec3,
		Vec4
	};

	enum class VertexInputRate
	{
		Vertex,
		Instance
	};

	struct VertexAttribute
	{
		uint32_t binding = 0;
		uint32_t location = 0;
		uint32_t offset = 0;
		DataFormat Format = DataFormat::Float;
	};

	struct VertexDescription
	{
		uint32_t stride = 0;
		VertexInputRate Rate = VertexInputRate::Vertex;
	};

	struct VertexFormat
	{
		std::vector<VertexDescription> Descriptions;
		std::vector<VertexAttribute> Attributes;
	};

	struct Vertex
	{
		glm::vec2 Position;
		glm::vec3 Color;
	};
}