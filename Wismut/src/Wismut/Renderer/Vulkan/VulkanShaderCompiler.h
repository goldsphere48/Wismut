#pragma once
#include "Wismut/Renderer/Shader.h"

namespace Wi
{
	class VulkanShaderCompiler
	{
	public:
		VulkanShaderCompiler(ShaderLanguage sourceLanguage);

		std::vector<uint32_t> CompileToSpv(const ShaderStageConfig& config);

	private:
		ShaderLanguage m_ShaderLanguage;
	};
}
