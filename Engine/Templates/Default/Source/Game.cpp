#include <Application/Application.h>
#include "Core/Logger/Logger.h"

class GameApp final : public Wi::Application
{
protected:
	void OnInit() override
	{
		Wi::Log::Info("Welcome from my game on my engine");
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
	return new GameApp();
}
