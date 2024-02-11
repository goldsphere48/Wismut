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
		uint32_t location = 0;
		uint32_t offset = 0;
		uint32_t stride = 0;
		DataFormat Format = DataFormat::Float;
		VertexInputRate Rate = VertexInputRate::Vertex;
	};

	using VertexFormat = std::vector<VertexAttribute>;

	struct Vertex
	{
		glm::vec2 Position;
	};
}