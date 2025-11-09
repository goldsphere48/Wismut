#pragma once
#ifdef WI_PLATFORM_ANDROID

#include "Platform/IPlatformWindow.h"
#include "Renderer/IRenderViewport.h"

struct ANativeWindow;

namespace Wi
{
	class AndroidWindow final : public IPlatformWindow
	{
	public:
		explicit AndroidWindow(const WindowDefinition& definition, ANativeWindow* window);

		void Destroy() override;
		uint16 GetWidth() override;
		uint16 GetHeight() override;
		void* GetNativeHandle() const override;
		IRenderViewport* GetViewport() override;

	private:
		IRenderViewport* m_Viewport;
		ANativeWindow* m_WindowHandle;
	};
}
#endif
