#include "wipch.h"

#include "Wismut/Core/Application.h"

#ifdef WI_PLATFORM_WIN

#include "WindowsVulkanPlatform.h"
#include <GLFW/glfw3.h>

namespace Wi
{
	std::vector<const char*> WindowsVulkanPlatform::GetExtensions()
	{
		std::vector extensions = 
		{
			"VK_KHR_surface",
			"VK_KHR_win32_surface"
		};

		return extensions;
	}

	vk::SurfaceKHR WindowsVulkanPlatform::CreateSurface(const vk::Instance& instance)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());
		VkSurfaceKHR surface;
		glfwCreateWindowSurface(instance, window, nullptr, &surface);
		return { surface };
	}

	std::pair<uint32_t, uint32_t> WindowsVulkanPlatform::GetFramebufferSize()
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow());
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		return { width, height };
	}
}
#endif
