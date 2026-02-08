#pragma once

#include "RenderTarget.h"
#include <glm/glm.hpp>
#include "AssetManager.h"
#include "Lights.h"

namespace ke
{
	class DirectionalShadow
	{
	private:
		RenderTarget m_renderTarget{};
		glm::mat4 m_lightSpaceMatrix{};
		int m_width{}, m_height{};
		int m_LightIndex{};
	public:
		DirectionalShadow(const glm::ivec2& size, AssetManager& assetManager, DirectionalLight& dirLight, int lightIndex);
		~DirectionalShadow() noexcept = default;

		const glm::mat4& getLightSpaceMatrix() const
		{
			return m_lightSpaceMatrix;
		}

		const RenderTarget& getShadowRenderTarget() const
		{
			return m_renderTarget;
		}

		Texture* getDepthMap(AssetManager& assetManager, int lightIndex) const
		{
			return assetManager.getTexture("shadow_map" + std::to_string(lightIndex));
		}

		void updateFromLight(const ke::DirectionalLight& dirLight);

		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }

		int getLightIndex() const { return m_LightIndex; }
		void bind() const;
	};
}
