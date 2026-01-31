#pragma once

#include <GL/glew.h>
#include <string>
#include <string_view>
#include <cstdint>
#include "stb/stb_image.h"
#include <stdexcept>
#include <cmath>
#include "EngineConfig.h"

namespace ke
{	
	class AssetManager;

	enum class TextureFormat
	{
		R8,
		RGB8,
		RGBA8,
		Depth24,
		Depth32
	};

	enum class TextureWrap
	{
		Repeat,
		ClampToEdge
	};

	enum TextureFilter
	{
		Linear,
		Nearest
	};

	struct TextureDesc
	{
		int width, height;
		TextureFormat format;
		TextureWrap wrapS = TextureWrap::Repeat;
		TextureWrap wrapT = TextureWrap::Repeat;
		TextureFilter minFilter = TextureFilter::Nearest;
		TextureFilter magFilter = TextureFilter::Nearest;
		void* data = nullptr;
	};

	class Texture
	{
	private:
		friend class AssetManager;

		GLuint m_id{};
	private:
		Texture(std::string_view path);
		Texture(const TextureDesc& desc);

		void loadFromFile(std::string_view path);
		void destroyTexture() noexcept;

		void createTexture(const TextureDesc& desc);
	public:
		
		~Texture() noexcept;

		void bind(ke::TextureSlot bindingPoint) const;

		GLuint getID() const { return m_id; }

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;
	};
}