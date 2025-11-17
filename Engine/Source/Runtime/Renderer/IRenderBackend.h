#pragma once
#include "IRenderViewport.h"
#include "Core/CoreTypes.h"
#include "Core/Math/Math.h"

namespace Wi
{
	using namespace Math;

	enum class VertexAttributeFormat : uint8_t
	{
		Float2,
		Float3,
		Float4,
	};

	struct VertexAttribute
	{
		uint32					Offset;
		uint32					Location;
		VertexAttributeFormat	Format;
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

		static constexpr VertexAttribute Attributes[] = {
			{
				.Offset = offsetof(Vertex, Position),
				.Location = 0,
				.Format = VertexAttributeFormat::Float3,
			},
			{
				.Offset = offsetof(Vertex, UV0),
				.Location = 3,
				.Format = VertexAttributeFormat::Float2,
			},
			{
				.Offset = offsetof(Vertex, UV1),
				.Location = 4,
				.Format = VertexAttributeFormat::Float2,
			},
			{
				.Offset = offsetof(Vertex, Color),
				.Location = 5,
				.Format = VertexAttributeFormat::Float4,
			},
		};
	};

#define DEFINE_HANDLER(name) \
	struct name##Handler \
	{ \
		template<typename T> \
		T* Cast() \
		{ \
			return static_cast<T*>(this); \
		} \
	};

	DEFINE_HANDLER(Buffer)
	DEFINE_HANDLER(Shader)

	class IRenderBackend
	{
	public:
		virtual ~IRenderBackend() = default;

		virtual IRenderViewport* CreateViewport(void* hwnd) const = 0;
		virtual void DestroyViewport(const IRenderViewport* viewport) const = 0;

		virtual void BindVertexBuffer(BufferHandler* buffer, int size);
		virtual void BindIndexBuffer(BufferHandler* buffer, int size);
		virtual void DrawIndexed(int count);

		virtual void ClearColor(float r, float g, float b) = 0;
	};
}
