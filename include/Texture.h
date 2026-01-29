#pragma once

#include <GL/glew.h>
#include <string>
#include <string_view>
#include <cstdint>
#include "stb/stb_image.h"
#include <stdexcept>
#include <cmath>

namespace ke
{	
	class AssetManager;

	class Texture
	{
	private:
		friend class AssetManager;

		GLuint m_id{};
	private:
		Texture(std::string_view path);

		void loadFromFile(std::string_view path);
		void destroyTexture() noexcept;
	public:
		
		~Texture() noexcept;

		void bind(uint32_t bindingPoint) const;

		GLuint getID() const { return m_id; }

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;
	};
}