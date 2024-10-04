#pragma once
#include "GenericPlatform/GenericApplication.h"

namespace Wi
{
	class WindowsApplication : public GenericApplication
	{
	public:
		WindowsApplication();
		~WindowsApplication() override;
	};
}
