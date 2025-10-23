#if WI_PLATFORM_ANDROID
#include "AndroidWindow.h"
#include <Renderer/Renderer.h>

namespace Wi
{
	AndroidWindow::AndroidWindow(const WindowDefinition& definition, ANativeWindow* window)
	{
		m_Viewport = Renderer::CreateViewport(window);
	}

	void AndroidWindow::Destroy()
	{
		Renderer::DestroyViewport(m_Viewport);
	}

	uint16 AndroidWindow::GetWidth()
	{
		return 0;
	}

	uint16 AndroidWindow::GetHeight()
	{
		return 0;
	}

	void* AndroidWindow::GetNativeHandle() const
	{
		return nullptr;
	}

	IRenderViewport* AndroidWindow::GetViewport()
	{
		return m_Viewport;
	}
}

#endif
