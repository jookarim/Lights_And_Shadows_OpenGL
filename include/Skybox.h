#pragma once

#include <GL/glew.h>
#include "stb/stb_image.h"
#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "EngineConfig.h"

namespace ke
{
	struct SkyboxDesc
	{
		std::vector<std::string> paths;
		TextureFilter minFilter = TextureFilter::Linear;
		TextureFilter magFilter = TextureFilter::Linear;
	};

	class AssetManager;

	class Skybox
	{
	private:
		GLuint m_id{};
		GLuint m_vao, m_vbo;
	private:

		friend class AssetManager;

		void loadFromFile(const SkyboxDesc& desc);
		void createBuffers();
		void destroy() noexcept;

		Skybox(const SkyboxDesc& desc);
	public:
		~Skybox() noexcept;
		
		Skybox(Skybox&& other) noexcept;
		Skybox& operator=(Skybox&& other) noexcept;

		Skybox(const Skybox&) = delete;
		Skybox& operator=(const Skybox&) = delete;

		void bind(ke::TextureSlot bindingIdx) const;
		
		GLuint getVAO() const { return m_vao; }
	};
}
