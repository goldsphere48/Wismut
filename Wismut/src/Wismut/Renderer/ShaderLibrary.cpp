#include "wipch.h"
#include "ShaderLibrary.h"

#include <ThirdParty/Toml/Toml.h>
#include <magic_enum/magic_enum.hpp>

#include "Renderer.h"


namespace Wi
{
	int SkipBOM(std::istream& in)
	{
		char test[4] = { 0 };
		in.seekg(0, std::ios::beg);
		in.read(test, 3);
		if (strcmp(test, "\xEF\xBB\xBF") == 0)
		{
			in.seekg(3, std::ios::beg);
			return 3;
		}
		in.seekg(0, std::ios::beg);
		return 0;
	}

	std::string ReadUTF8FileWithoutBOM(const std::filesystem::path& path)
	{
		std::string result;
		std::ifstream in(path, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			std::streampos fileSize = in.tellg();
			const int skippedChars = SkipBOM(in);

			fileSize -= skippedChars - 1;
			result.resize(fileSize);
			in.read(result.data() + 1, fileSize);
			// Add a dummy tab to beginning of file.
			result[0] = '\t';
		}
		in.close();
		return result;
	}

	void ShaderLibrary::Load(const std::filesystem::path& configPath)
	{
		const ShaderConfig config = ShaderLoader::ParseConfig(configPath);

		if (m_Shaders.contains(config.Name))
		{
			WI_CORE_WARN("Shader {0} is already loaded. Maybe you need invoke Reload instead ?", config.Name);
			return;
		}

		m_Shaders[config.Name] = Renderer::CreateShaderProgram(config);
	}

	void ShaderLibrary::Destroy()
	{
		for (const std::shared_ptr<Shader>& shader : m_Shaders | std::views::values)
			Renderer::DestroyShaderProgram(shader);
		
		m_Shaders.clear();
	}

	static std::string MakeRelativeToCfg(const std::filesystem::path& cfgPath, const std::filesystem::path& shaderPath)
	{
		const std::filesystem::path absCfgPath = std::filesystem::absolute(cfgPath);
		const std::filesystem::path cfgDirectory = absCfgPath.parent_path();

		std::filesystem::path absShaderPath;

		if (shaderPath.is_absolute())
			absShaderPath = shaderPath;
		else
			absShaderPath = cfgDirectory / shaderPath;

		return std::filesystem::relative(absShaderPath, std::filesystem::current_path()).generic_string();
	}

	ShaderConfig ShaderLoader::ParseConfig(const std::filesystem::path& cfg)
	{
		ShaderConfig config;
		const std::string source = ReadUTF8FileWithoutBOM(cfg);
		const toml::parse_result result = toml::parse(source);

		WI_CORE_ASSERT(result, "Faield to open shader config {0}. Error: {1}", cfg.generic_string(), result.error().description());
		
		config.Name = cfg.stem().generic_string();
		config.Language = ParseLanguage(result);
		config.Stages = ParseStages(cfg, result);

		return config;
	}

	ShaderLanguage ShaderLoader::ParseLanguage(const toml::table& table)
	{
		const auto language = table["Language"].as<std::string>();
		WI_CORE_ASSERT(language, "Shader language undeclared");

		const auto languageOptional = magic_enum::enum_cast<ShaderLanguage>(language->get());
		WI_CORE_ASSERT(languageOptional.has_value(), "Failed to cast string to enum <language>");

		return languageOptional.value_or(ShaderLanguage::None);
	}

	std::vector<ShaderStageConfig> ShaderLoader::ParseStages(const std::filesystem::path& cfg, const toml::table& table)
	{
		const auto stagesTable = table["Stages"].as_table();
		WI_CORE_ASSERT(stagesTable, "Stages table is undeclared");

		std::vector<ShaderStageConfig> stages;

		for (const std::string_view stage : magic_enum::enum_names<ShaderStage>())
		{
			if (stagesTable->contains(stage))
			{
				ShaderStageConfig config;
				const auto stageTable = stagesTable->at(stage).as_table();
				const auto relativePath = stageTable->at("Source").as<std::string>()->get();
				const std::filesystem::path shaderPath(relativePath);
				config.Stage = magic_enum::enum_cast<ShaderStage>(stage).value_or(ShaderStage::None);
				config.SourceFilePath = MakeRelativeToCfg(cfg, shaderPath);
				config.SourceFileName = shaderPath.filename().generic_string();
				config.Entry = stageTable->at("Entry").as<std::string>()->get();
				config.Source = ReadUTF8FileWithoutBOM(config.SourceFilePath);
				stages.push_back(config);
			}
		}

		return stages;
	}
}
