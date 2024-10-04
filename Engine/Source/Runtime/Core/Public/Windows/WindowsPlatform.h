#pragma once
#include "GenericPlatform/GenericPlatform.h"
#include "Windows/WindowsApplication.h"

namespace Wi
{
	struct WindowsPlatform
	{
		struct PlatformTypes : GenericPlatformTypes
		{

		};
	};

	using Application = WindowsApplication;
	using Platform = WindowsPlatform;
	using PlatformTypes = WindowsPlatform::PlatformTypes;
}
