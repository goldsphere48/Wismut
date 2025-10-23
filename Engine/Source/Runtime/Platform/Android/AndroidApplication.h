#pragma once
#ifdef WI_PLATFORM_ANDROID

#include "Platform/IPlatformApplication.h"
#include "Platform/IPlatformWindow.h"

struct android_app;

namespace Wi
{
	class AndroidApplication final : public IPlatformApplication
	{
	public:
		void Startup() override;
		SharedPtr<IPlatformWindow> MakeWindow(const WindowDefinition& config) override;
		void PumpMessages() override;
		void Shutdown() override;

		static UniquePtr<IPlatformApplication> CreateApplication();

		static void InitializeNativeApp(android_app* app);
	};

	using PlatformApplication = AndroidApplication;
}
#endif
