#include "AssetManager.h"

namespace ke
{
	Texture* AssetManager::loadTexture(std::string_view name, std::string_view path)
	{
		std::string key{ name };

		auto it = m_textures.find(key);
		if (it != m_textures.end())
			return it->second.get();

		auto texture = std::unique_ptr<Texture>(new Texture(path));
		Texture* raw = texture.get();       

		m_textures.emplace(key, std::move(texture));
		return raw;
	}

	void AssetManager::destroyTexture(std::string_view name) noexcept
	{
		auto key = std::string(name);

		if (m_textures.find(key) != m_textures.end())
			m_textures.erase(std::string(name));
	}

	Texture* AssetManager::getTexture(std::string_view name)
	{
		auto key = std::string(name);

		if (m_textures.find(key) != m_textures.end())
		{
			return m_textures[key].get();
		}

		return nullptr;
	}

	Shader* AssetManager::loadShader(std::string_view name, const ShaderDesc& desc)
	{
		auto key = std::string(name);

		if (m_shaders.find(key) != m_shaders.end())
		{
			return m_shaders[key].get();
		}

		auto shader = std::unique_ptr<Shader>(new Shader(desc));

		m_shaders[key] = std::move(shader);

		return m_shaders[key].get();
	}

	void AssetManager::destroyShader(std::string_view name) noexcept
	{
		auto key = std::string(name);

		if (m_shaders.find(key) != m_shaders.end())
			m_shaders.erase(key);
	}

	Shader* AssetManager::getShader(std::string_view name)
	{
		auto key = std::string(name);

		if (m_shaders.find(key) != m_shaders.end())
		{
			return m_shaders[key].get();
		}

		return nullptr;
	}

}
