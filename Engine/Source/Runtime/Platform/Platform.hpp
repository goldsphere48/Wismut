#pragma once
#include "IPlatformApplication.hpp"
#include "Core/Defines.hpp"

namespace Wi
{
	class Platform
	{
	public:
		static UniquePtr<IPlatformApplication> CreateNativeApplication();
	};
}
