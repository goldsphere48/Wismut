#pragma once
#include <glm/glm.hpp>

namespace Wi
{
	struct Handler
	{
		template<typename T>
		__forceinline T* AsStatic()
		{
			return static_cast<T*>(this);
		}
	};

#define DEFINE_HANDLER(name)			\
		struct name##Handler : Handler	\
		{								\
										\
		}								\

	DEFINE_HANDLER(Buffer);
	DEFINE_HANDLER(RenderPass);
	DEFINE_HANDLER(Pipeline);
	DEFINE_HANDLER(Shader);

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

	enum class BufferUsageFlagBits
	{
		Vertex
	};

	struct Buffer
	{
		uint32_t Size;
		BufferHandler* Handler;
	};

	struct RenderPassSpecification
	{

	};

	struct RenderPass
	{
		RenderPassHandler* Handler;
	};

	enum class ShaderStage
	{
		None,
		Vertex,
		Fragment
	};

	enum class ShaderLanguage
	{
		None,
		GLSL,
		HLSL
	};

	struct ShaderStageConfig
	{
		std::string SourceFilePath;
		std::string SourceFileName;
		std::string Source;
		std::string Entry;
		ShaderStage Stage;
	};

	struct ShaderConfig
	{
		ShaderLanguage Language;
		std::vector<ShaderStageConfig> Stages;
		std::string Name;
	};

	struct Shader
	{
		ShaderHandler* Handler;
	};

	struct PipelineSpecification
	{
		Shader* Shader;
		VertexFormat VertexFormat;
	};

	struct GraphicsPipeline
	{
		PipelineHandler* Handler;
	};
}