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

		static VertexFormat CreateFormat()
		{
			return {
				.Descriptions = {
					VertexDescription {
					.stride = sizeof(Vertex),
					.Rate = VertexInputRate::Vertex,
					}
				},
				.Attributes = {
					VertexAttribute {
						.binding = 0,
						.location = 0,
						.offset = offsetof(Vertex, Position),
						.Format = DataFormat::Vec2,
					},
					VertexAttribute {
						.binding = 0,
						.location = 1,
						.offset = offsetof(Vertex, Color),
						.Format = DataFormat::Vec3,
					}
				},
			};
		}
	};

	enum class BufferType
	{
		Vertex,
		Index,
		Staging,
		Uniform
	};

	struct Buffer
	{
		uint32_t Size;
		BufferHandler* Handler;
	};

	struct IndexBuffer : Buffer
	{
		uint32_t Count;
	};

	struct UniformBuffer : Buffer
	{
		void* MappedData;
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

	enum class DescriptorType
	{
		UniformBuffer,
		MAX
	};

	struct ShaderUniform
	{
		std::string Name = "";
		DescriptorType Type = DescriptorType::MAX;
		uint32_t Binding = 0;
	};

	struct ShaderStageDescription
	{
		size_t GlobalUboStride = 0;
		std::unordered_map<uint32_t, ShaderUniform> Uniforms;
	};

	using ShaderDescription = std::unordered_map<ShaderStage, ShaderStageDescription>;

	class ShaderStageBinary
	{
	public:
		virtual ~ShaderStageBinary() = default;

		template<typename T>
		__forceinline T* GetData() { return static_cast<T*>(GetDataInternal()); }
		virtual size_t GetSize() const = 0;
		virtual size_t GetCount() const = 0;

	private:
		virtual void* GetDataInternal() const = 0;
	};

	using ShaderBinary = std::unordered_map<ShaderStage, ShaderStageBinary*>;

	struct Shader
	{
		ShaderHandler* Handler;
		ShaderDescription Description;
		std::shared_ptr<UniformBuffer> UniformBuffer;
	};

	struct PipelineSpecification
	{
		std::shared_ptr<Shader> Shader;
		VertexFormat VertexFormat;
	};

	struct GraphicsPipeline
	{
		PipelineHandler* Handler;
		std::shared_ptr<Shader> Shader;
	};

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};
}