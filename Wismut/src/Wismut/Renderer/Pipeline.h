#pragma once

#include "RendererCommon.h"
#include "RenderPass.h"
#include "Shader.h"

namespace Wi
{
	struct PipelineSpecification
	{
		Shader* Shader;
		RenderPass* RenderPass;
		VertexFormat VertexFormat;
	};

	struct GraphicsPipeline
	{
		
	};
}
