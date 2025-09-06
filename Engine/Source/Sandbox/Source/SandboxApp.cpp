#include <Core/Entry.h>

class SandboxApp : public Wi::Application
{
};

Wi::Application* CreateApplication()
{
	return WI_NEW SandboxApp();
}
