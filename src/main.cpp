#include "Engine.h"

#define MAX_DIR_LIGHTS 16

int main()
{
	try
	{
		ke::WindowDesc windowDesc{};
		windowDesc.width = ke::AppConfig::WindowWidth;
		windowDesc.height = ke::AppConfig::WindowHeight;
		windowDesc.title = ke::AppConfig::WindowTitle;
		windowDesc.iconPath = ke::AppConfig::WindowIcon;

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

		ke::RenderState renderState{};
		renderState.cullEnabled = false;
		renderState.depthFunc = ke::DepthFunc::Less;
		renderState.depthTest = true;
		renderState.depthWrite = true;
		renderState.clearColor = { 1.f, 0.f, 0.f, 1.f };

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
		camera.position = { 0.f, 20.f, 30.f };
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

		dirLight.ambient = glm::vec3(0.08f);

		dirLight.diffuse = glm::vec3(0.9f);

		dirLight.specular = glm::vec3(0.25f);

		dirLight.direction = glm::normalize(glm::vec3(
			-0.3f,   
			-1.0f,   
			-0.2f    
		));

		std::vector<ke::DirectionalLight> dirLights = { dirLight };

		ke::ShaderStorageBuffer storageBuffer(MAX_DIR_LIGHTS * sizeof(ke::DirectionalLight), 1);
		storageBuffer.uploadData(dirLights.size() * sizeof(ke::DirectionalLight), dirLights.data());


		ke::SkyboxDesc skyboxDesc;
		skyboxDesc.minFilter = ke::TextureFilter::Linear;
		skyboxDesc.magFilter = ke::TextureFilter::Linear;
		skyboxDesc.paths = {
			"assets/images/skybox/nightRight.png",
			"assets/images/skybox/nightLeft.png",
			"assets/images/skybox/nightTop.png",
			"assets/images/skybox/nightBottom.png",
			"assets/images/skybox/nightFront.png",
			"assets/images/skybox/nightBack.png",
		};

		auto skybox = assetManager.loadSkybox("skybox", skyboxDesc);

		ke::ShaderDesc skyboxShaderDesc;
		skyboxShaderDesc.vertPath = "assets/shaders/skybox.vert";
		skyboxShaderDesc.fragPath = "assets/shaders/skybox.frag";

		auto skyboxShader = assetManager.loadShader("skybox_shader", skyboxShaderDesc);

		ke::RenderState skyboxRenderState;
		skyboxRenderState.depthTest = true;
		skyboxRenderState.depthWrite = false;
		skyboxRenderState.cullEnabled = false;
		skyboxRenderState.depthFunc = ke::DepthFunc::LessEqual;

		while (!window.shouldClose())
		{
			window.pollEvents();

			renderTarget.bind();

			ke::RenderCommand::Clear(ke::ClearCommand::Color | ke::ClearCommand::Depth);

			ke::RenderCommand::ApplyRenderState(skyboxRenderState);

			skyboxShader->bind();

			skyboxShader->setUniformMatrix4("u_View", camera.getViewMatrix());

			skyboxShader->setUniformMatrix4("u_Proj", camera.getProjectionMatrix(window.getWidth(), window.getHeight()));

			skybox->bind(ke::TextureSlot::Skybox);
			
			ke::RenderCommand::DrawSkybox(skybox->getVAO());

			ke::RenderCommand::ApplyRenderState(renderState);

			shader->bind();

			shader->setUniformMatrix4("u_MVP",camera.getProjectionMatrix(window.getWidth(), window.getHeight()) * camera.getViewMatrix() * transform.getModelMatrix());

			shader->setUniformMatrix4("u_Model", transform.getModelMatrix());

			shader->setUniformVec3("viewPos", camera.position);

			shader->setUniformInt("dirLightsCount", static_cast<int>(dirLights.size()));

			texture->bind(ke::TextureSlot::Albedo);

			ke::RenderCommand::DrawIndexed(mesh.getVAO(),mesh.getIndexCount());

			ke::RenderCommand::BindDefaultFramebuffer();

			ke::RenderCommand::Clear(ke::ClearCommand::Color | ke::ClearCommand::Depth);

			proceduralTexture->bind(ke::TextureSlot::GrayScale);

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