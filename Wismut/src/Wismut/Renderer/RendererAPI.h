#pragma once

namespace Wi
{
	enum class RendererAPIType
	{
		None,
		Vulkan	
	};

	class RendererAPI
	{
	public:
		RendererAPI(RendererAPIType renderAPIType) { s_CurrentAPIType = renderAPIType; };

		static RendererAPIType Current();
	private:
		inline static RendererAPIType s_CurrentAPIType;
	};
}

