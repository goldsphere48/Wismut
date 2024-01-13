#include "wipch.h"
#include "Renderer.h"

#include "RendererContext.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanRenderAPI.h"

namespace Wi
{
	static std::unique_ptr<RendererAPI> CreateRenderAPI()
	{
		return std::make_unique<VulkanRenderAPI>();
	}

	static std::unique_ptr<RendererContext> CreateRenderModule()
	{
		return std::make_unique<VulkanContext>();
	}
	
	void Renderer::Initialize()
	{
		WI_CORE_INFO("Initializing renderer...")

		s_RendererContext = CreateRenderModule();
		s_RenderAPI = CreateRenderAPI();

		s_RendererContext->Initialize();
	}

	void Renderer::Shutdown()
	{
		s_RendererContext->Destroy();
	}
}
