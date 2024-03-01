#pragma once
#include "Wismut/Renderer/RendererCommon.h"
#include "Wismut/Renderer/ShaderLibrary.h"

namespace Wi
{
	class SpirvShaderBinary : public ShaderStageBinary
	{
	public:
		SpirvShaderBinary(const std::vector<uint32_t>& data) : m_Data(data)
		{
			
		}

		size_t GetSize() const override { return m_Data.size() * sizeof(uint32_t); }
		size_t GetCount() const override { return m_Data.size(); }

	private:
		void* GetDataInternal() const override { return (void*)m_Data.data(); }

		std::vector<uint32_t> m_Data;
	};

	class VulkanShaderCompiler : public ShaderCompiler
	{
	public:
		ShaderBinary Compile(const ShaderConfig& config) const override;

	private:
		std::vector<uint32_t> CompileToSpv(const ShaderStageConfig& config, ShaderLanguage sourceLanguage) const;
		std::vector<uint32_t> CompileGLSLToSpv(const ShaderStageConfig& config) const;
	};
}
