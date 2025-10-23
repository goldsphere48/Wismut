#if WI_PLATFORM_ANDROID
#include "AndroidApplication.h"
#include "AndroidWindow.h"
#include "Core/Assertion.h"
#include <android_native_app_glue.h>

namespace Wi
{
	android_app* GNativeApp = nullptr;

	void AndroidApplication::Startup()
	{
	}

	SharedPtr<IPlatformWindow> AndroidApplication::MakeWindow(const WindowDefinition& config)
	{
		return CreateSharedPtr<AndroidWindow>(config, GNativeApp->window);
	}

	void AndroidApplication::PumpMessages()
	{
	}

	void AndroidApplication::Shutdown()
	{
	}

	UniquePtr<IPlatformApplication> AndroidApplication::CreateApplication()
	{
		return CreateUniquePtr<AndroidApplication>();
	}

	void AndroidApplication::InitializeNativeApp(android_app* app)
	{
		WI_ASSERT(!GNativeApp)

		GNativeApp = app;
	}
}

#endif
