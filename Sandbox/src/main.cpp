#include "wipch.h"

#include <Wismut.h>
#include <Wismut/EntryPoint.h>

class MyLayer : public Wi::Layer
{
public:
	void OnAttach() override
	{
		WI_INFO("Layer attached")
	}

	void Update() override
	{
		Wi::Renderer::DrawTest();
	}
};

class MyApplication : public Wi::Application
{
public:
	MyApplication(const Wi::ApplicationConfig& config) : Wi::Application(config)
	{
		PushLayer(new MyLayer());
	}
};

Wi::Application* CreateApplication()
{
	const Wi::ApplicationConfig config = {
		.ApplicationName = "Sandbox",
		.WindowWidth = 1024,
		.WindowHeight = 768,
		.RendererConfig = {
			.MaxFramesInFlight = 3
		}
	};

	return new MyApplication(config);
}