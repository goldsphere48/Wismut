#pragma once
#include "Pipeline.h"
#include "Shader.h"

namespace Wi
{
	enum class RendererAPIType
	{
		None,
		Vulkan	
	};

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;
		RendererAPI(RendererAPIType renderAPIType) { s_CurrentAPIType = renderAPIType; }
		static RendererAPIType Current();

		virtual std::shared_ptr<Shader> CreateShaderProgram(const ShaderConfig& config) const = 0;
		virtual void DestroyShaderProgram(const std::shared_ptr<Shader>& shader) const = 0;

	private:
		inline static RendererAPIType s_CurrentAPIType;
	};
}

