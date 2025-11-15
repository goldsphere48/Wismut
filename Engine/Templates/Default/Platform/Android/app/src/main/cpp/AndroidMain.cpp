#include <android_native_app_glue.h>

#include <Application/Entry.h>
#include <Platform/Android/AndroidApplication.h>

#include <android/looper.h>

extern "C"
{
	void android_main(android_app* state)
	{
		app_dummy();

		int events;
		android_poll_source* source;

		while (!state->window)
		{
			if (ALooper_pollOnce(-1, nullptr, &events, (void**)&source) >= 0)
			{
				if (source)
					source->process(state, source);
			}

			if (state->destroyRequested)
				return;
		}

		Wi::AndroidApplication::InitializeNativeApp(state);
		Wi::RunEngine();
	}
}
