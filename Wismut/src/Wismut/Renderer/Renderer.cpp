#include "wipch.h"
#include "Renderer.h"

#include "RendererContext.h"
#include "Vulkan/VulkanContext.h"
#include "Wismut/Core/Application.h"

namespace Wi
{
	static RendererContext* CreateRenderModule()
	{
		return new VulkanContext();
	}
	
	void Renderer::Initialize(const RendererConfig& config)
	{
		s_RendererConfig = config;
		WI_CORE_INFO("Initializing renderer...")

		s_RendererContext = CreateRenderModule();
		s_RendererContext->Initialize();
		s_RendererConfig.MaxFramesInFlight = std::min(s_RendererConfig.MaxFramesInFlight, s_RendererContext->GetAvailableImagesCount());

		s_RenderAPI = s_RendererContext->GetApi();

		s_Library.Load("Resources/Shaders/Base.shadercfg");

		const std::vector<Vertex> vertices = {
			{
				.Position = {0.0f, -0.5f },
				.Color = {1.0f, 0.0f, 0.0f }
			},
			{
				.Position = {0.5f, 0.5f },
				.Color = {0.0f, 1.0f, 0.0f }
			},
			{
				.Position = {-0.5f, 0.5f },
				.Color = {0.0f, 0.0f, 1.0f }
			},
		};

		const VertexFormat format = {
			.Descriptions = {
				VertexDescription {
				.stride = sizeof(Vertex),
				.Rate = VertexInputRate::Vertex,
				}
			},
			.Attributes = {
				VertexAttribute {
					.binding = 0,
					.location = 0,
					.offset = offsetof(Vertex, Position),
					.Format = DataFormat::Vec2,
				},
				VertexAttribute {
					.binding = 0,
					.location = 1,
					.offset = offsetof(Vertex, Color),
					.Format = DataFormat::Vec3,
				}
			},
		};

		const PipelineSpecification pipelineSpecification = {
			.Shader = s_Library.Get("Base").get(),
			.VertexFormat = format
		};

		m_Pipeline = s_RenderAPI->CreateGraphicsPipeline(pipelineSpecification);
		m_Buffer = s_RenderAPI->CreateBuffer(vertices, BufferUsageFlagBits::Vertex);
	}

	void Renderer::Shutdown()
	{
		s_RenderAPI->DestroyBuffer(m_Buffer);
		s_RenderAPI->DestroyGraphicsPipeline(m_Pipeline);

		s_Library.Destroy();
		s_RendererContext->Destroy();
		delete s_RendererContext;
	}

	void Renderer::Begin()
	{
		s_RenderAPI->BeginFrame();
		s_RenderAPI->BeginRenderPass();
	}

	void Renderer::End()
	{
		s_RenderAPI->EndRenderPass();
		s_RenderAPI->EndFrame();
	}

	void Renderer::OnWindowResize(int width, int height)
	{
		s_RendererContext->OnWindowResize(width, height);
	}

	void Renderer::DrawTest()
	{
		s_RenderAPI->RenderGeometry(m_Pipeline, m_Buffer);
	}

	uint32_t Renderer::GetCurrentFrameIndex()
	{
		return Application::Get()->GetCurrentFrameIndex();
	}
}
