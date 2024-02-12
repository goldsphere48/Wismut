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
	MyApplication()
	{
		PushLayer(new MyLayer());
	}
};

Wi::Application* CreateApplication()
{
	return new MyApplication();
}