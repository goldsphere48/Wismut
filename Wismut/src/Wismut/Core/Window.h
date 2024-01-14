#pragma once
#include "Events/Event.h"

namespace Wi
{
	class WindowSpecification
	{
	public:
		std::string Title;
		int Width;
		int Height;
		EventCallback EventCallback;
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void PollEvents() = 0;
		virtual void Destroy() = 0;
		virtual void* GetNativeWindow() = 0;

	public:
		static std::shared_ptr<Window> Create(const WindowSpecification& specification);
	};
}
