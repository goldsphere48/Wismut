#pragma once

namespace Wi
{
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

	};
}
