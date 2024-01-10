#include <Wismut.h>
#include <Wismut/EntryPoint.h>

class MyLayer : public Wi::Layer
{
public:
	void OnAttach() override
	{
		WI_INFO("Layer attached")
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