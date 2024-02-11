#include "wipch.h"
#include "Renderer.h"

#include "RendererContext.h"
#include "Vulkan/VulkanContext.h"

namespace Wi
{
	static RendererContext* CreateRenderModule()
	{
		return new VulkanContext();
	}
	
	void Renderer::Initialize()
	{
		WI_CORE_INFO("Initializing renderer...")

		s_RendererContext = CreateRenderModule();
		s_RendererContext->Initialize();
		s_RenderAPI = s_RendererContext->GetApi();

		s_Library.Load("Resources/Shaders/Base.shadercfg");

		RenderPassSpecification renderPassSpecification = {

		};

		const auto renderPass = s_RenderAPI->CreateRenderPass(renderPassSpecification);

		constexpr Vertex vertices[] = {
			{
				.Position = {0.0f, -0.5f }
			},
			{
				.Position = {0.5f, 0.5f }
			},
			{
				.Position = {-0.5f, 0.5f }
			},
		};

		const VertexFormat format = {
			VertexAttribute {
				.location = 0,
				.offset = offsetof(Vertex, Position),
				.stride = sizeof(Vertex),
				.Format = DataFormat::Float,
				.Rate = VertexInputRate::Vertex,
			}
		};

		const PipelineSpecification pipelineSpecification = {
			.Shader = s_Library.Get("Base").get(),
			.RenderPass = renderPass.get(),
			.VertexFormat = format
		};

		const auto pipeline = s_RenderAPI->CreateGraphicsPipeline(pipelineSpecification);

		const auto buffer = s_RenderAPI->CreateBuffer(sizeof(vertices), BufferUsageFlagBits::Vertex);
		s_RenderAPI->DestroyBuffer(buffer);

		s_RenderAPI->DestroyGraphicsPipeline(pipeline);
		s_RenderAPI->DestroyRenderPass(renderPass);
	}

	void Renderer::Shutdown()
	{
		s_Library.Destroy();
		s_RendererContext->Destroy();
		delete s_RendererContext;
	}
}
