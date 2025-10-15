#pragma once

namespace Wi
{
	struct RenderConfig
	{
		static const RenderConfig& GetInstance()
		{
			static RenderConfig instance;
			return instance;
		}

		bool VSync;
		static constexpr bool IsDebugEnvironment()
		{
#ifdef WI_DEBUG
			return true;
#else
			return false;
#endif
		}
	};
}
