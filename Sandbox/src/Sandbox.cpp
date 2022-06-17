#include <Wismut.h>

class Sandbox : public Wi::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox() override
	{
		
	}
};

Wi::Application* Wi::CreateApplication()
{
	WI_WARN("Initialize App");
	return new Sandbox();
}