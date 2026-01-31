#include "Texture.h"

namespace ke
{
	void Texture::loadFromFile(std::string_view path)
	{
		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(true);

		stbi_uc* data = stbi_load(path.data(), &width, &height, &nrChannels, 0);

		if (!data) throw std::runtime_error("Failed to load texture: " + std::string(path));

		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	
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

	void Texture::bind(ke::TextureSlot bindingPoint) const
	{
		glBindTextureUnit(static_cast<uint32_t>(bindingPoint), m_id);
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

	Texture::Texture(const TextureDesc& desc)
	{
		createTexture(desc);
	}

	void Texture::createTexture(const TextureDesc& desc)
	{
		if (desc.width == 0 || desc.height == 0) throw std::runtime_error("Width and Height are invalid");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

		GLenum dataFormat;
		GLenum internalFormat;
		GLenum dataType;

		if (desc.format == TextureFormat::Depth24)
		{
			dataFormat = GL_DEPTH_COMPONENT;
			internalFormat = GL_DEPTH_COMPONENT24;
			dataType = GL_FLOAT;
		}

		else if (desc.format == TextureFormat::Depth32)
		{
			dataFormat = GL_DEPTH_COMPONENT;
			internalFormat = GL_DEPTH_COMPONENT32;
			dataType = GL_FLOAT;
		}

		else if (desc.format == TextureFormat::R8)
		{
			dataFormat = GL_RED;
			internalFormat = GL_R8;
			dataType = GL_UNSIGNED_BYTE;
		}

		else if (desc.format == TextureFormat::RGB8)
		{
			dataFormat = GL_RGB;
			internalFormat = GL_RGB8;
			dataType = GL_UNSIGNED_BYTE;
		}

		else if (desc.format == TextureFormat::RGBA8)
		{
			dataFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
			dataType = GL_UNSIGNED_BYTE;
		}

		glTextureStorage2D(m_id, 1, internalFormat, desc.width, desc.height);
		glTextureSubImage2D(m_id, 0, 0, 0, desc.width, desc.height, dataFormat, dataType, desc.data);

		GLenum minFilter = GL_NEAREST , magFilter = GL_NEAREST;

		if (desc.minFilter == TextureFilter::Linear)
		{
			minFilter = GL_LINEAR;
		}

		else if (desc.minFilter == TextureFilter::Nearest)
		{
			minFilter = GL_NEAREST;
		}

		if (desc.magFilter == TextureFilter::Linear)
		{
			magFilter = GL_LINEAR;
		}

		else if (desc.magFilter == TextureFilter::Nearest)
		{
			magFilter = GL_NEAREST;
		}

		GLenum wrapS;
		GLenum wrapT;

		if (desc.wrapS == TextureWrap::Repeat)
		{
			wrapS = GL_REPEAT;
		}

		else if (desc.wrapS == TextureWrap::ClampToEdge)
		{
			wrapS = GL_CLAMP_TO_EDGE;
		}

		if (desc.wrapT == TextureWrap::Repeat)
		{
			wrapT = GL_REPEAT;
		}

		else if (desc.wrapT == TextureWrap::ClampToEdge)
		{
			wrapT = GL_CLAMP_TO_EDGE;
		}

		glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, minFilter);
		glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, magFilter);
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, wrapS);
		glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, wrapT);
	}
}