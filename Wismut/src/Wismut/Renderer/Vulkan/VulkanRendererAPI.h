#pragma once
#include "VulkanDevice.h"
#include "Wismut/Renderer/RendererAPI.h"

namespace Wi
{
	class VulkanContext;

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI(VulkanContext* context);
		std::shared_ptr<Shader> CreateShaderProgram(const ShaderConfig& config) const override;
		void DestroyShaderProgram(const std::shared_ptr<Shader>& shader) const override;

	private:
		VulkanContext* m_Context;
		vk::Device m_Device;
	};
}
