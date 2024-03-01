#pragma once
#include <ThirdParty/Toml/Toml.h>

#include "RendererCommon.h"

namespace Wi
{
	class ShaderLoader
	{
	public:
		static ShaderConfig ParseConfig(const std::filesystem::path& cfg);

	private:
		static ShaderLanguage ParseLanguage(const toml::table& table);
		static std::vector<ShaderStageConfig> ParseStages(const std::filesystem::path& cfg, const toml::table& table);
	};

	class ShaderCompiler
	{
	public:
		virtual ~ShaderCompiler() = default;

		virtual ShaderBinary Compile(const ShaderConfig& config) const = 0;
	};

	class ShaderLibrary
	{
	public:
		void Load(const std::filesystem::path& configPath);
		void Destroy();

		ShaderCompiler* GetCompiler() const;

		std::shared_ptr<Shader> Get(const std::string& name) const { return m_Shaders.at(name); }

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}
