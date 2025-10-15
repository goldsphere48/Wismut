#include "Renderer.h"

#include "Core/Logger/Logger.h"
#include "OpenGL/GLRenderBackend.h"
#include "OpenGL/GLViewport.h"

namespace Wi
{
	UniquePtr<IRenderBackend> Renderer::s_RenderBackend = nullptr;

	void Renderer::Initialize()
	{
		Log::Info("Renderer initialization...");

		s_RenderBackend = CreateUniquePtr<GLRenderBackend>();

		Log::Info("Renderer initialized");
	}

	void Renderer::Shutdown()
	{
		Log::Info("Renderer destroyed");
	}

	IRenderViewport* Renderer::CreateViewport(void* handle)
	{
		return s_RenderBackend->CreateViewport(handle);
	}

	void Renderer::DestroyViewport(const IRenderViewport* viewport)
	{
		s_RenderBackend->DestroyViewport(viewport);
	}
}
