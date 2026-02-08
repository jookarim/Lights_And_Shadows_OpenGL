#include "DirectionalShadow.h"
#include "AssetManager.h"

namespace ke
{
	DirectionalShadow::DirectionalShadow(const glm::ivec2& size, AssetManager& assetManager, DirectionalLight& dirLight, int index)
	{
		m_LightIndex = index;

		TextureDesc depthMapDesc;
		depthMapDesc.data = nullptr;
		depthMapDesc.format = TextureFormat::Depth24;
		depthMapDesc.width = size.x;
		depthMapDesc.height = size.y;
		depthMapDesc.minFilter = TextureFilter::Nearest;
		depthMapDesc.magFilter = TextureFilter::Nearest;
		depthMapDesc.wrapS = TextureWrap::ClampToEdge;
		depthMapDesc.wrapT = TextureWrap::ClampToEdge;

		auto depthMap = assetManager.createTexture("shadow_map" + std::to_string(m_LightIndex), depthMapDesc);

		RenderTargetDesc renderTargetDesc;
		renderTargetDesc.width = size.x;
		renderTargetDesc.height = size.y;
		renderTargetDesc.depthAttachment = depthMap;

		m_renderTarget.attachData(renderTargetDesc);

		float orthoSize = 100.0f;
		float nearPlane = 1.0f;
		float farPlane = 50.0f;

		glm::mat4 lightProj = glm::ortho(
			-orthoSize, orthoSize,
			-orthoSize, orthoSize,
			nearPlane, farPlane
		);

		glm::vec3 lightPos = -glm::normalize(dirLight.direction) * 20.0f; 

		glm::mat4 lightView = glm::lookAt(
			lightPos,
			glm::vec3(0.0f),      
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		m_lightSpaceMatrix = lightProj * lightView;

		dirLight.lightSpaceMatrix = m_lightSpaceMatrix;

		m_width = size.x;
		m_height = size.y;
	}

	void DirectionalShadow::updateFromLight(const ke::DirectionalLight& dirLight)
	{
		float orthoSize = 100.0f;
		float nearPlane = 1.0f;
		float farPlane = 50.0f;
		glm::mat4 lightProj = glm::ortho(
			-orthoSize, orthoSize,
			-orthoSize, orthoSize,
			nearPlane, farPlane
		);
		glm::vec3 lightPos = -glm::normalize(dirLight.direction) * 20.0f; 
		glm::mat4 lightView = glm::lookAt(
			lightPos,
			glm::vec3(0.0f),      
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		m_lightSpaceMatrix = lightProj * lightView;
	}

	void DirectionalShadow::bind() const
	{
		m_renderTarget.bind();
	}
}