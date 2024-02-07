#pragma once
#include <ThirdParty/Toml/Toml.h>

#include "Shader.h"

namespace Wi
{
	class ShaderLoader
	{
	public:
		static ShaderConfig ParseConfig(const std::filesystem::path& cfg);

	private:
		static ShaderLanguage ParseLanguage(const toml::table& table);
		static std::unordered_map<ShaderStage, ShaderStageConfig> ParseStages(const std::filesystem::path& cfg, const toml::table& table);
	};

	class ShaderLibrary
	{
	public:
		void Load(const std::filesystem::path& configPath);
		void Destroy();

		std::shared_ptr<Shader> Get(const std::string& name) const { return m_Shaders.at(name); }

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
	};
}
