#include "GLUtils.h"
#include "Core/Logger/Logger.h"
#include "Renderer/RenderConfig.h"

#include <glad/glad.h>

namespace Wi::Debug::OpenGL
{
	static const char* SourceToString(GLenum source)
	{
		switch (source)
		{
			case GL_DEBUG_SOURCE_API:				return "GL_DEBUG_SOURCE_API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "GL_DEBUG_SOURCE_SHADER_COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY:		return "GL_DEBUG_SOURCE_THIRD_PARTY";
			case GL_DEBUG_SOURCE_APPLICATION:		return "GL_DEBUG_SOURCE_APPLICATION";
			case GL_DEBUG_SOURCE_OTHER:				return "GL_DEBUG_SOURCE_OTHER";
			default:								return "UnknownSrc";
		}
	}

	static const char* TypeToString(GLenum type)
	{
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:				return "GL_DEBUG_TYPE_ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY:			return "GL_DEBUG_TYPE_PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE:			return "GL_DEBUG_TYPE_PERFORMANCE";
			case GL_DEBUG_TYPE_MARKER:				return "GL_DEBUG_TYPE_MARKER";
			case GL_DEBUG_TYPE_PUSH_GROUP:			return "GL_DEBUG_TYPE_PUSH_GROUP";
			case GL_DEBUG_TYPE_POP_GROUP:			return "GL_DEBUG_TYPE_POP_GROUP";
			case GL_DEBUG_TYPE_OTHER:				return "GL_DEBUG_TYPE_OTHER";
			default:								return "UnknownType";
		}
	}

	static const char* SeverityToString(GLenum severity)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_LOW:		return "LOW";
			case GL_DEBUG_SEVERITY_MEDIUM:	return "MEDIUM";
			case GL_DEBUG_SEVERITY_HIGH:	return "HIGH";
			default:						return "UnknownSeverity";
		}
	}

	static void GLAPIENTRY OpenGLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
	{
		if (severity > GL_DEBUG_SEVERITY_NOTIFICATION)
		{
			const char* sourceStr = SourceToString(source);
			const char* typeStr = TypeToString(type);
			const char* severityStr = SeverityToString(severity);

			if (severity == GL_DEBUG_SEVERITY_LOW)
				Log::Warn("Severity: {} Source: {} Type: {} ID: {} {}", severityStr, sourceStr, typeStr, id, message);
			else
				Log::Error("Severity: {} Source: {} Type: {} ID: {} {}", severityStr, sourceStr, typeStr, id, message);
		}
	}

	void InitDebugContext()
	{
		if constexpr (RenderConfig::IsDebugEnvironment())
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLDebugMessageCallback, nullptr);
		}
	}
}
