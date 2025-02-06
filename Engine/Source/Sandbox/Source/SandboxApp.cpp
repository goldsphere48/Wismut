#include <Core/Entry.h>

class SandboxApp : public Wi::Application
{
};

Wi::Application* CreateApplication()
{
	return new SandboxApp();
}
