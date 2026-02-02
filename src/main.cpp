#include <iostream>
#include "Window.h"
#include "Mesh.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "AssetManager.h"
#include "Transform.h"
#include "Camera.h"
#include "RenderState.h"
#include "EngineConfig.h"
#include "RenderTarget.h"
#include "ShaderStorageBuffer.h"
#include "Lights.h"

int main()
{
	try
	{
		ke::WindowDesc windowDesc{};
		windowDesc.width = ke::AppConfig::WindowWidth;
		windowDesc.height = ke::AppConfig::WindowHeight;
		windowDesc.title = ke::AppConfig::WindowTitle;

		ke::Window window(windowDesc, {});

		ke::MeshData meshData{};

		meshData.vertices = {
			{{-5.f, 0.f, -5.f}, {0.f, 0.f}, {0.f, 1.f, 0.f}},
			{{ 5.f, 0.f, -5.f}, {1.f, 0.f}, {0.f, 1.f, 0.f}}, 
			{{ 5.f, 0.f,  5.f}, {1.f, 1.f}, {0.f, 1.f, 0.f}}, 
			{{-5.f, 0.f,  5.f}, {0.f, 1.f}, {0.f, 1.f, 0.f}}  
		};

		meshData.indices = {
			0, 1, 2,
			2, 3, 0
		};

		ke::Mesh mesh(meshData);

		ke::RenderCommand::ClearColor(1.f, 1.f, 0.f, 1.f);

		ke::RenderState renderState{};
		renderState.cullEnabled = false;
		renderState.depthFunc = ke::DepthFunc::Less;
		renderState.depthTest = true;
		renderState.depthWrite = true;

		ke::RenderCommand::ApplyRenderState(renderState);

		auto& assetManager = ke::AssetManager::getInstance();

		ke::ShaderDesc shaderDesc{};
		shaderDesc.vertPath = "assets/shaders/shader.vert";
		shaderDesc.fragPath = "assets/shaders/shader.frag";

		auto shader = assetManager.loadShader("shader", shaderDesc);

		ke::LoadTextureDesc textureDesc{};
		textureDesc.path = "assets/images/bricks2.jpg";
		textureDesc.magFilter = ke::TextureFilter::Linear;
		textureDesc.minFilter = ke::TextureFilter::Linear;
		textureDesc.wrapS = ke::TextureWrap::Repeat;
		textureDesc.wrapT = ke::TextureWrap::Repeat;
		
		auto texture = assetManager.loadTexture("texture", textureDesc);

		ke::Transform transform{};
		transform.position = { 0.f, 1.f, 0.f };
		transform.scale = { 0.5f, 0.5f, 0.5f };
		transform.rotation = { 0.f, 0.f, 0.f };

		ke::Camera camera{};

		camera.near = ke::CameraDefaults::Near;
		camera.far = ke::CameraDefaults::Far;
		camera.position = { 0.f, 5.f, 10.f };
		camera.forward = glm::normalize(glm::vec3(0.f, -0.5f, -1.f));
		camera.up = { 0.f, 1.f, 0.f };
		camera.fov = ke::CameraDefaults::Fov;

		ke::TextureDesc proceduralTextureDesc{};

		proceduralTextureDesc.format = ke::TextureFormat::RGB8;
		proceduralTextureDesc.width = window.getWidth();
		proceduralTextureDesc.height = window.getHeight();
		proceduralTextureDesc.magFilter = ke::TextureFilter::Nearest;
		proceduralTextureDesc.minFilter = ke::TextureFilter::Nearest;
		proceduralTextureDesc.wrapS = ke::TextureWrap::Repeat;
		proceduralTextureDesc.wrapT = ke::TextureWrap::Repeat;
		proceduralTextureDesc.data = nullptr;

		auto proceduralTexture = assetManager.createTexture("procedural_texture", proceduralTextureDesc);

		ke::RenderTargetDesc fboDesc{};
		fboDesc.width = window.getWidth();
		fboDesc.height = window.getHeight();
		fboDesc.colorAttachments = { proceduralTexture };

		ke::RenderTarget renderTarget;
		renderTarget.attachData(fboDesc);

		ke::ShaderDesc grayShaderDesc{};
		grayShaderDesc.vertPath = "assets/shaders/fullscreen.vert";
		grayShaderDesc.fragPath = "assets/shaders/fullscreen.frag";

		auto grayScaleShader = assetManager.loadShader("grayscale_shader", grayShaderDesc);

		ke::DirectionalLight dirLight{};
		dirLight.ambient = glm::vec3(0.2f);
		dirLight.diffuse = glm::vec3(1.f, 1.f, 1.f);
		dirLight.specular = glm::vec3(1.f, 1.f, 1.f);
		dirLight.direction = glm::normalize(glm::vec3(0.4f, -0.6f, 0.3f));

		std::vector<ke::DirectionalLight> dirLights = { dirLight };

		ke::ShaderStorageBuffer storageBuffer(dirLights.size() * sizeof(ke::DirectionalLight), 1);

		storageBuffer.uploadData(dirLights.size() * sizeof(ke::DirectionalLight), dirLights.data());

		while (!window.shouldClose())
		{
			window.pollEvents();

			renderTarget.bind();

			ke::RenderCommand::Clear(ke::ClearCommand::Color | ke::ClearCommand::Depth);

			shader->bind();

			shader->setUniformMatrix4(
				"u_MVP",
				camera.getProjectionMatrix(window.getWidth(), window.getHeight()) *
				camera.getViewMatrix() *
				transform.getModelMatrix()
			);

			shader->setUniformMatrix4("u_Model", transform.getModelMatrix());

			shader->setUniformVec3("viewPos", camera.position);

			texture->bind(ke::TextureSlot::Albedo);

			ke::RenderCommand::DrawIndexed(mesh.getVAO(),mesh.getIndexCount());

			ke::RenderCommand::BindDefaultFramebuffer();

			ke::RenderCommand::Clear(ke::ClearCommand::Color | ke::ClearCommand::Depth);

			proceduralTexture->bind(ke::TextureSlot::Albedo);

			grayScaleShader->bind();

			ke::RenderCommand::DrawFullscreenQuad();

			window.swapBuffers();
		}
	}
	
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
}