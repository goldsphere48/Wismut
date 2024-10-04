#include <EntryPoint.h>

using namespace Wi;

class LauncherApplication final : public Application
{
};

Application *CreateApplication()
{
	return new LauncherApplication();
}
