#pragma once
#include "Wismut/Renderer/RendererConfig.h"

namespace Wi
{
	struct ApplicationConfig
	{
		std::string ApplicationName = "Wismut App";
		uint32_t WindowWidth = 1024;
		uint32_t WindowHeight = 768;
		RendererConfig RendererConfig;
	};
}
