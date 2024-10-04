#include <Core/EntryPoint.h>

class LauncherApplication final : public Wi::Application
{
};

Wi::Application* CreateApplication()
{
	return new LauncherApplication();
}
