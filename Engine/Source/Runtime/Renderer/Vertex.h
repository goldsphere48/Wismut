#pragma once
#include <cstddef>

#include "Core/Math/Math.h"
#include "Core/CoreTypes.h"

namespace Wi
{
	using namespace Math;

	enum class VertexAttributeSemantic : uint8_t
	{
		Position,
		UV0,
		UV1,
		Color
	};

	enum class VertexAttributeFormat : uint8_t
	{
		Float2,
		Float3,
		Float4,
	};

	struct VertexAttributeDesc
	{
		VertexAttributeSemantic Semantic;
		VertexAttributeFormat	Format;
		usize					Offset;
		usize					Location;
	};

	struct Vertex
	{
		Vec3 Position;
		Vec2 UV0;
		Vec2 UV1;
		Vec4 Color;
	};

	struct VertexLayout
	{
		static constexpr uint32 Stride = sizeof(Vertex);

		static constexpr VertexAttributeDesc Attributes[] = {
			{
				VertexAttributeSemantic::Position, 
				VertexAttributeFormat::Float3, 
				offsetof(Vertex, Position),
				0
			},
			{ 
				VertexAttributeSemantic::UV0, 
				VertexAttributeFormat::Float2, 
				offsetof(Vertex, UV0), 
				3 
			},
			{ 
				VertexAttributeSemantic::UV1, 
				VertexAttributeFormat::Float2, 
				offsetof(Vertex, UV1), 
				4 
			},
			{ 
				VertexAttributeSemantic::Color, 
				VertexAttributeFormat::Float4, 
				offsetof(Vertex, Color), 
				5 
			},
		};
	};
}
