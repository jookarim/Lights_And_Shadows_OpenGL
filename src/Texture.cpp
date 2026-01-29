#include "Texture.h"

namespace ke
{
	void Texture::loadFromFile(std::string_view path)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

		int width, height, nrChannels;
		
		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(path.data(), &width, &height, &nrChannels, 0);

		if (!data) throw std::runtime_error("Failed to load texture: " + std::string(path));

		GLenum internalFormat = GL_R8;
		GLenum dataFormat = GL_RED;

		if (nrChannels == 1)
		{
			internalFormat = GL_R8;
			dataFormat = GL_RED;
		}

		else if (nrChannels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		else if (nrChannels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		glTextureStorage2D(m_id, 1, internalFormat, width, height);
		glTextureSubImage2D(m_id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);

		stbi_image_free(data);
	}

	void Texture::destroyTexture() noexcept
	{
		if (m_id)
		{
			glDeleteTextures(1, &m_id);
			m_id = 0;
		}
	}

	Texture::~Texture() noexcept
	{
		destroyTexture();
	}

	Texture::Texture(std::string_view path)
	{
		loadFromFile(path);
	}

	void Texture::bind(uint32_t bindingPoint) const
	{
		glBindTextureUnit(bindingPoint, m_id);
	}

	Texture::Texture(Texture&& other) noexcept
	{
		m_id = other.m_id;
		other.m_id = 0;
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this == &other) return *this;
		destroyTexture();
		m_id = other.m_id;
		other.m_id = 0;
		return *this;
	}
}