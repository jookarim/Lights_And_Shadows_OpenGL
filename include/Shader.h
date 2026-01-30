#pragma once

#include <GL/glew.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EngineConfig.h"

namespace ke
{
	struct ShaderDesc
	{
		std::string vertPath = "";
		std::string fragPath = "";
	};

	class AssetManager;

	class Shader
	{
	private:
		friend class AssetManager;

		Shader(const ShaderDesc& desc);

		std::string loadFromFile(std::string_view path);

		GLuint createShader(std::string_view code, GLenum type);
		void createProgram(GLuint& vertex, GLuint& fragment);
		void destroyProgram() noexcept;
		void destroyShaders(GLuint& vertex, GLuint& fragment) noexcept;
	private:
		GLuint m_program{};
		mutable std::unordered_map<std::string, GLint> m_uniformCache{};

		GLint getLocation(std::string_view name);
	public:
		~Shader() noexcept;

		void bind() const;

		void setUniformMatrix4(std::string_view name, const glm::mat4& value);
		void setUniformInt(std::string_view name, int value);
		
		void setUniformTexture(std::string_view name, TextureSlot slot);

		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;
	};
}