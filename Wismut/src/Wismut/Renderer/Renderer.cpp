#include "wipch.h"
#include "Renderer.h"

#include "RendererContext.h"
#include "Vulkan/VulkanContext.h"
#include "Vulkan/VulkanRendererAPI.h"

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
	}

	void Renderer::Shutdown()
	{
		s_Library.Destroy();
		s_RendererContext->Destroy();
		delete s_RendererContext;
	}
}
