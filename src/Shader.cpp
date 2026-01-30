#include "Shader.h"
#include <stdexcept>

namespace ke
{
	GLuint Shader::createShader(std::string_view code, GLenum type)
	{
		GLuint shader = glCreateShader(type);
		
		std::string source(code);
		const char* cCode = source.c_str();

		glShaderSource(shader, 1, &cCode, nullptr);
		glCompileShader(shader);

		GLint success = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			char infoLog[1024];
			glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

			glDeleteShader(shader);
			throw std::runtime_error(infoLog);
		}

		return shader;
	}


	std::string Shader::loadFromFile(std::string_view path)
	{
		std::ifstream file(path.data());

		if (!file.good())
		{
			throw std::runtime_error("Failed to open file" + std::string(path));
		}

		std::stringstream ss;

		ss << file.rdbuf();

		std::string data = ss.str();
		
		if (data.empty())
		{
			throw std::runtime_error("File is empty: " + std::string(path));
		}
		return data;
	}

	void Shader::createProgram(GLuint& vertex, GLuint& fragment)
	{
		m_program = glCreateProgram();

		glAttachShader(m_program, vertex);
		glAttachShader(m_program, fragment);

		glLinkProgram(m_program);

		GLint success;

		glGetProgramiv(m_program, GL_LINK_STATUS, &success);

		if (!success)
		{
			GLchar infoLog[512];

			glGetProgramInfoLog(m_program, 512, nullptr, infoLog);

			glDeleteProgram(m_program);
			throw std::runtime_error(infoLog);
		}
		
		
		glDetachShader(m_program, vertex);
		glDetachShader(m_program, fragment);

		destroyShaders(vertex, fragment);
	}

	void Shader::destroyProgram() noexcept
	{
		if (m_program)
		{
			glDeleteProgram(m_program);
			m_program = 0;
		}
	}

	Shader::~Shader() noexcept
	{
		destroyProgram();
	}

	void Shader::destroyShaders(GLuint& vert, GLuint& frag) noexcept
	{
		if (vert)
		{
			glDeleteShader(vert);
			vert = 0;
		}

		if (frag)
		{
			glDeleteShader(frag);
			frag = 0;
		}
	}

	GLint Shader::getLocation(std::string_view name) 
	{
		auto key = std::string(name);

		if (m_uniformCache.find(key) != m_uniformCache.end())
		{
			return m_uniformCache.at(key);
		}

		GLint location = glGetUniformLocation(m_program, name.data());

		m_uniformCache[key] = location;

		return location;
	}

	void Shader::bind() const
	{
		glUseProgram(m_program);
	}

	Shader::Shader(const ShaderDesc& desc)
	{
		std::string vertCode = loadFromFile(desc.vertPath);
		std::string fragCode = loadFromFile(desc.fragPath);

		GLuint vertShader = createShader(vertCode, GL_VERTEX_SHADER);
		GLuint fragShader = createShader(fragCode, GL_FRAGMENT_SHADER);

		createProgram(vertShader, fragShader);
	}

	Shader::Shader(Shader&& other) noexcept
	{
		m_program = other.m_program;
		m_uniformCache = std::move(other.m_uniformCache);

		other.m_program = 0;
		other.m_uniformCache = {};
	}

	Shader& Shader::operator=(Shader&& other) noexcept
	{
		if (this == &other) return *this;
		
		destroyProgram();

		m_program = other.m_program;
		m_uniformCache = std::move(other.m_uniformCache);

		other.m_program = 0;
		other.m_uniformCache = {};

		return *this;
	}

	void Shader::setUniformMatrix4(std::string_view name, const glm::mat4& value)
	{
		GLint location = getLocation(name);

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void Shader::setUniformInt(std::string_view name, int value)
	{
		GLint location = getLocation(name);

		glUniform1i(location, value);
	}

	void Shader::setUniformTexture(std::string_view name, ke::TextureSlot slot)
	{
		setUniformInt(name, static_cast<int>(slot));
	}
}