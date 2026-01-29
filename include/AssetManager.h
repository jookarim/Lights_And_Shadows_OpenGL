#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <string_view>
#include "Texture.h"
#include "Shader.h"
#include <stdexcept>

namespace ke
{
	class AssetManager
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures{};
		std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders{};
	public:
		AssetManager() = default;

		Texture* loadTexture(std::string_view name, std::string_view path);
		void destroyTexture(std::string_view name) noexcept;
		Texture* getTexture(std::string_view name);

		Shader* loadShader(std::string_view name, const ShaderDesc& desc);
		void destroyShader(std::string_view name) noexcept;
		Shader* getShader(std::string_view name);

		AssetManager(const AssetManager&) = delete;
		AssetManager& operator=(const AssetManager&) = delete;
		AssetManager(AssetManager&&) = delete;
		AssetManager& operator=(AssetManager&&) = delete;
		
		static AssetManager& getInstance()
		{
			static AssetManager assetManager;
			return assetManager;
		}
	};
}