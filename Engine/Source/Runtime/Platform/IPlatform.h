#pragma once

/*------------------------------------------------------------------------------
	Platform common interface.
------------------------------------------------------------------------------*/

#include "Core.h"
#include "Platform/IPlatformApplication.h"

namespace Wi::Platform
{
	UniquePtr<IPlatformApplication> CreateNativeApplication();
}
