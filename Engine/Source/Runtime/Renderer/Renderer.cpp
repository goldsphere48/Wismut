#include "Renderer.h"

#include "Core/Logger/Logger.h"
#include "OpenGL/GLRenderBackend.h"

namespace Wi
{
	UniquePtr<IRenderBackend> Renderer::s_RenderBackend = nullptr;

	void Renderer::Initialize()
	{
		Log::Info("Renderer initialization...");

#ifdef WI_OPENGL_BUILTIN
		s_RenderBackend = CreateUniquePtr<GLRenderBackend>();
#else
	#error "Implement something better..."
#endif

		Log::Info("Renderer initialized");
	}

	void Renderer::Shutdown()
	{
		Log::Info("Renderer destroyed");
	}
}
