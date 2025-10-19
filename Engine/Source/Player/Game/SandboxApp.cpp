#include <Application/Application.h>

#include "Application/Input.h"
#include "Application/Events/KeyEvent.h"
#include "Core/Logger/Logger.h"

class SandboxApp final : public Wi::Application
{
protected:
	void OnInit() override
	{
		
	}

	void OnUpdate() override
	{
		
	}

	void OnEvent(Wi::Event& event) override
	{
		
	}
};

Wi::Application* Wi::CreateApplication()
{
	return new SandboxApp();
}
