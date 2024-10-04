#include <EntryPoint.h>

class LauncherApplication final : public Wi::GenericApplication
{
};

Wi::GenericApplication *CreateApplication()
{
	return new LauncherApplication();
}
