#pragma once

namespace Wi
{
	class RendererAPI;

	class RendererContext
	{
	public:
		virtual ~RendererContext() = default;

		virtual void Initialize() = 0;
		virtual void Destroy() = 0;

		virtual RendererAPI* GetApi() const = 0;
		virtual void OnWindowResize(int width, int height) = 0;
	};
}

