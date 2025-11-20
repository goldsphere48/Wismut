#include "Renderer.h"

#include "Core/Logger/Logger.h"
#include "OpenGL/GLRenderBackend.h"

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
}
