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
				{{0.f, 1.f, 0.f}, {0.f, 1.f}, {0.f, 0.f, 1.f}},
				{{1.f, -1.f, 0.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}},
				{{-1.f, -1.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}}
		};

		meshData.indices = {
				0,1,2
		};

		ke::Mesh mesh(meshData);

		ke::RenderCommand::ClearColor(1.f, 1.f, 0.f, 1.f);

		ke::ShaderDesc shaderDesc{};
		shaderDesc.vertPath = "assets/shaders/shader.vert";
		shaderDesc.fragPath = "assets/shaders/shader.frag";

		ke::RenderState renderState{};
		renderState.cullEnabled = false;
		renderState.depthFunc = ke::DepthFunc::Less;
		renderState.depthTest = true;
		renderState.depthWrite = true;

		ke::RenderCommand::ApplyRenderState(renderState);

		auto& assetManager = ke::AssetManager::getInstance();

		auto shader = assetManager.loadShader("shader", shaderDesc);
		auto texture = assetManager.loadTexture("texture", "assets/images/brick.png");

		ke::Transform transform{};
		transform.position = { 0.f, 1.f, 0.f };
		transform.scale = { 0.5f, 0.5f, 0.5f };
		transform.rotation = { 0.f, 0.f, 45.f };

		ke::Camera camera{};

		camera.near = ke::CameraDefaults::Near;
		camera.far = ke::CameraDefaults::Far;
		camera.position = { 0.f, 0.f, 10.f };
		camera.forward = { 0.f, 0.f, -1.f };
		camera.up = { 0.f, 1.f, 0.f };
		camera.fov = ke::CameraDefaults::Fov;

		ke::TextureDesc textureDesc{};

		textureDesc.format = ke::TextureFormat::RGB8;
		textureDesc.width = window.getWidth();
		textureDesc.height = window.getHeight();
		textureDesc.magFilter = ke::TextureFilter::Nearest;
		textureDesc.minFilter = ke::TextureFilter::Nearest;
		textureDesc.wrapS = ke::TextureWrap::Repeat;
		textureDesc.wrapT = ke::TextureWrap::Repeat;
		textureDesc.data = nullptr;

		auto proceduralTexture = assetManager.createTexture("procedural_texture", textureDesc);

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

			texture->bind(ke::TextureSlot::Albedo);

			ke::RenderCommand::DrawIndexed(mesh.getVAO(),mesh.getIndexCount());

			ke::RenderCommand::BindDefaultFramebuffer();

			ke::RenderCommand::Clear(ke::ClearCommand::Color | ke::ClearCommand::Depth);

			grayScaleShader->bind();

			proceduralTexture->bind(ke::TextureSlot::Albedo);

			ke::RenderCommand::DrawFullscreenQuad();

			window.swapBuffers();
		}
	}
	
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	
}