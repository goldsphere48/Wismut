#include <Wismut.h>
#include <Wismut/EntryPoint.h>

class MyApplication : public Wi::Application
{
	
};

Wi::Application* CreateApplication()
{
	return new MyApplication();
}