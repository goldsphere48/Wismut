#include <Core/Entry.hpp>

class SandboxApp : public Wi::Application
{
};

Wi::Application* CreateApplication()
{
	return new SandboxApp();
}
