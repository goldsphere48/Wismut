#include "wipch.h"
#include "VulkanShaderCompiler.h"

#include <magic_enum/magic_enum.hpp>
#include <shaderc/shaderc.hpp>

namespace Wi
{
	shaderc_shader_kind ToShaderCGLSLStage(ShaderStage stage)
	{
		switch (stage)
		{
			case ShaderStage::Fragment:
				return shaderc_shader_kind::shaderc_glsl_fragment_shader;
			case ShaderStage::Vertex: 
				return shaderc_shader_kind::shaderc_glsl_vertex_shader;
		}

		return static_cast<shaderc_shader_kind>(0);
	}

	VulkanShaderCompiler::VulkanShaderCompiler(ShaderLanguage sourceLanguage) : m_ShaderLanguage(sourceLanguage)
	{
	}

	std::vector<uint32_t> VulkanShaderCompiler::CompileToSpv(const ShaderStageConfig& config)
	{
		shaderc::Compiler compiler;
		shaderc::CompileOptions options;

		WI_CORE_INFO("Compiling shader {0} stage {1} ...", config.SourceFilePath, magic_enum::enum_name(config.Stage));

		const shaderc::SpvCompilationResult result = compiler.CompileGlslToSpv(config.Source, ToShaderCGLSLStage(config.Stage), config.SourceFilePath.c_str(), options);

		WI_CORE_ASSERT(result.GetCompilationStatus() == shaderc_compilation_status_success, "Failed to compile glsl shader to spirv assembly. Error: {0}", result.GetErrorMessage());

		WI_CORE_INFO("Success");

		return { result.begin(), result.end() };
	}
}
