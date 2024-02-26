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
				.Position = {-0.5f, -0.5f },
				.Color = {1.0f, 0.0f, 0.0f }
			},
			{
				.Position = {0.5f, -0.5f },
				.Color = {0.0f, 1.0f, 0.0f }
			},
			{
				.Position = {0.5f, 0.5f },
				.Color = {0.0f, 0.0f, 1.0f }
			},
			{
				.Position = {-0.5f, 0.5f },
				.Color = {0.0f, 0.0f, 1.0f }
			},
		};

		const std::vector<uint32_t> indicies = {
			0, 1, 2, 2, 3, 0
		};

		const PipelineSpecification pipelineSpecification = {
			.Shader = s_Library.Get("Base").get(),
			.VertexFormat = Vertex::CreateFormat()
		};

		m_Pipeline = CreateGraphicsPipeline(pipelineSpecification);
		m_Buffer = CreateVertexBuffer(vertices);
		m_IndexBuffer = CreateIndexBuffer(indicies);
	}

	void Renderer::BeginFrame()
	{
		s_RenderAPI->BeginFrame();
		s_RenderAPI->BeginRenderPass();
	}

	void Renderer::EndFrame()
	{
		s_RenderAPI->EndRenderPass();
		s_RenderAPI->EndFrame();
	}

	void Renderer::OnWindowResize(int width, int height)
	{
		s_RendererContext->OnWindowResize(width, height);
	}

	std::shared_ptr<Buffer> Renderer::CreateVertexBuffer(const std::vector<Vertex>& data)
	{
		const std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>();
		buffer->Size = data.size() * sizeof(Vertex);
		buffer->Handler = s_RenderAPI->CreateBuffer(buffer->Size, BufferType::Vertex);

		BufferHandler* stagingBuffer = s_RenderAPI->CreateBuffer(buffer->Size, BufferType::Staging);
		uint8_t* pData = s_RenderAPI->MapBuffer(stagingBuffer, buffer->Size);
		memcpy(pData, data.data(), buffer->Size);
		s_RenderAPI->UnmapBuffer(stagingBuffer);

		s_RenderAPI->CopyBuffer(stagingBuffer, buffer->Handler, buffer->Size);
		s_RenderAPI->DestroyBuffer(stagingBuffer);

		return buffer;
	}

	std::shared_ptr<IndexBuffer> Renderer::CreateIndexBuffer(const std::vector<uint32_t>& data)
	{
		const std::shared_ptr<IndexBuffer> buffer = std::make_shared<IndexBuffer>();
		buffer->Size = data.size() * sizeof(Vertex);
		buffer->Count = static_cast<uint32_t>(data.size());
		buffer->Handler = s_RenderAPI->CreateBuffer(buffer->Size, BufferType::Index);

		BufferHandler* stagingBuffer = s_RenderAPI->CreateBuffer(buffer->Size, BufferType::Staging);
		uint8_t* pData = s_RenderAPI->MapBuffer(stagingBuffer, buffer->Size);
		memcpy(pData, data.data(), buffer->Size);
		s_RenderAPI->UnmapBuffer(stagingBuffer);

		s_RenderAPI->CopyBuffer(stagingBuffer, buffer->Handler, buffer->Size);
		s_RenderAPI->DestroyBuffer(stagingBuffer);

		return buffer;
	}

	std::shared_ptr<GraphicsPipeline> Renderer::CreateGraphicsPipeline(const PipelineSpecification& specification)
	{
		const std::shared_ptr<GraphicsPipeline> pipeline = std::make_shared<GraphicsPipeline>();
		pipeline->Handler = s_RenderAPI->CreateGraphicsPipeline(specification);
		return pipeline;
	}

	std::shared_ptr<RenderPass> Renderer::CreateRenderPass(const RenderPassSpecification& specification)
	{
		const std::shared_ptr<RenderPass> renderPass = std::make_shared<RenderPass>();
		renderPass->Handler = s_RenderAPI->CreateRenderPass(specification);
		return renderPass;
	}

	std::shared_ptr<Shader> Renderer::CreateShaderProgram(const ShaderConfig& config)
	{
		const std::shared_ptr<Shader> shader = std::make_shared<Shader>();
		shader->Handler = s_RenderAPI->CreateShaderProgram(config);
		return shader;
	}

	void Renderer::DestroyShaderProgram(const std::shared_ptr<Shader>& shader)
	{
		s_RenderAPI->DestroyShaderProgram(shader->Handler);
	}

	void Renderer::DrawTest()
	{
		s_RenderAPI->BindPipeline(m_Pipeline);
		s_RenderAPI->BindVertexBuffer(m_Buffer);
		s_RenderAPI->BindIndexBuffer(m_IndexBuffer);
		s_RenderAPI->DrawIndexed(m_IndexBuffer->Count);
	}

	uint32_t Renderer::GetCurrentFrameIndex()
	{
		return Application::Get()->GetCurrentFrameIndex();
	}

	void Renderer::Shutdown()
	{
		s_RenderAPI->DestroyBuffer(m_Buffer->Handler);
		s_RenderAPI->DestroyBuffer(m_IndexBuffer->Handler);
		s_RenderAPI->DestroyGraphicsPipeline(m_Pipeline->Handler);

		s_Library.Destroy();
		s_RendererContext->Destroy();
		delete s_RendererContext;
	}
}
