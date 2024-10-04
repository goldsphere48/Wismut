#pragma once

#ifdef WI_PLATFORM_WIN
#include "Windows/WindowsPlatform.h"
#else
#error "Unsupported platform"
#endif

namespace Wi
{
	using uint64 = PlatformTypes::uint64;
	using uint32 = PlatformTypes::uint32;
	using uint16 = PlatformTypes::uint16;
	using uint8  = PlatformTypes::uint8;
	using int64  = PlatformTypes::int64;
	using int32  = PlatformTypes::int32;
	using int16  = PlatformTypes::int16;
	using int8   = PlatformTypes::int8;
}
