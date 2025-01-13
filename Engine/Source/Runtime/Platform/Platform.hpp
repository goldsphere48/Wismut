#pragma once
#include "Application/INativeApplication.hpp"
#include "Core/Defines.hpp"

namespace Wi
{
	class Platform
	{
	public:
		static UniquePtr<INativeApplication> CreateNativeApplication();
	};
}
