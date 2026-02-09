#include "Engine.h"

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

		ke::calculateTangent(meshData.vertices, meshData.indices);

		ke::Mesh mesh(meshData);

		ke::Mesh sphereMesh(ke::generateSphere());

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

		ke::LoadTextureDesc normalTextureDesc;
		normalTextureDesc.path = "assets/images/bricks2_normal.jpg";
		normalTextureDesc.minFilter = ke::TextureFilter::Linear;
		normalTextureDesc.magFilter = ke::TextureFilter::Linear;
		normalTextureDesc.wrapS = ke::TextureWrap::Repeat;
		normalTextureDesc.wrapT = ke::TextureWrap::Repeat;

		auto normalMap = assetManager.loadTexture("normal_map", normalTextureDesc);

		ke::LoadTextureDesc sphereTextureDesc{};
		sphereTextureDesc.path = "assets/images/rock_diffuse.png";
		sphereTextureDesc.magFilter = ke::TextureFilter::Linear;
		sphereTextureDesc.minFilter = ke::TextureFilter::Linear;
		sphereTextureDesc.wrapS = ke::TextureWrap::Repeat;
		sphereTextureDesc.wrapT = ke::TextureWrap::Repeat;

		auto sphereTexture = assetManager.loadTexture("rock_texture", sphereTextureDesc);

		ke::LoadTextureDesc sphereNormalTextureDesc;
		sphereNormalTextureDesc.path = "assets/images/rock_normal.jpg";
		sphereNormalTextureDesc.minFilter = ke::TextureFilter::Linear;
		sphereNormalTextureDesc.magFilter = ke::TextureFilter::Linear;
		sphereNormalTextureDesc.wrapS = ke::TextureWrap::Repeat;
		sphereNormalTextureDesc.wrapT = ke::TextureWrap::Repeat;

		auto sphereNormalMap = assetManager.loadTexture("rock_normal", sphereNormalTextureDesc);

		ke::Transform transform{};
		transform.position = { 0.f, 1.f, 0.f };
		transform.scale = { 5.f, 1.f, 5.f };
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

		ke::TextureDesc depthDesc{};
		depthDesc.format = ke::TextureFormat::Depth24;
		depthDesc.width = window.getWidth();
		depthDesc.height = window.getHeight();
		depthDesc.data = nullptr;

		auto depthTexture = assetManager.createTexture("grayscale_depth", depthDesc);

		fboDesc.depthAttachment = depthTexture;

		ke::RenderTarget renderTarget;
		renderTarget.attachData(fboDesc);
		
		ke::ShaderDesc grayShaderDesc{};
		grayShaderDesc.vertPath = "assets/shaders/fullscreen.vert";
		grayShaderDesc.fragPath = "assets/shaders/fullscreen.frag";

		auto grayScaleShader = assetManager.loadShader("grayscale_shader", grayShaderDesc);

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

		ke::DirectionalLight dirLight{};

		dirLight.ambient = glm::vec3(0.08f);

		dirLight.diffuse = glm::vec3(0.9f);

		dirLight.specular = glm::vec3(1.f);

		dirLight.direction = glm::normalize(glm::vec3{ -0.365f, -0.714f, -0.183f });


		ke::DirectionalLight dirLight2{};

		dirLight2.ambient = glm::vec3(0.08f);

		dirLight2.diffuse = glm::vec3(1.f);

		dirLight2.specular = glm::vec3(1.f);

		dirLight2.direction = glm::normalize(glm::vec3(
			-2.143f,
			-2.857f,
			0.282f
		));


		ke::PointLight pointLight{};

		pointLight.ambient = glm::vec3(0.1f, 0.0f, 0.0f);
		pointLight.diffuse = glm::vec3(1.f);
		pointLight.specular = glm::vec3(1.f);

		pointLight.position = glm::vec3(2.5f, 1.f, 3.f);

		pointLight.constant = 1.0f;
		pointLight.linear = 0.045f;
		pointLight.quadric = 0.0075f;

		std::vector<std::unique_ptr<ke::DirectionalShadow>> shadows;

		shadows.emplace_back(
			std::make_unique<ke::DirectionalShadow>(
				glm::ivec2{ 2048, 2048 }, assetManager, dirLight, 0
			)
		);

		shadows.emplace_back(
			std::make_unique<ke::DirectionalShadow>(
				glm::ivec2{ 2048, 2048 }, assetManager, dirLight2, 1
			)
		);

		std::vector<ke::DirectionalLight> dirLights = { dirLight, dirLight2 };

		ke::ShaderDesc shadowShaderDesc;
		shadowShaderDesc.vertPath = "assets/shaders/shadow.vert";
		shadowShaderDesc.fragPath = "assets/shaders/shadow.frag";

		auto shadowShader = assetManager.loadShader("shadow_shader", shadowShaderDesc);

		ke::Transform sphereTransform{};
		sphereTransform.position = { 3.f, 10.f, 0.f };   
		sphereTransform.scale = { 0.5f, 0.5f, 0.5f };      
		sphereTransform.rotation = { 0.f, 0.f, 0.f };

		ke::ShaderStorageBuffer storageBuffer(MAX_DIR_LIGHTS * sizeof(ke::DirectionalLight), 3);

		std::vector<ke::PointLight> pointLights = { pointLight };

		ke::ShaderStorageBuffer pointStorageBuffer(MAX_POINT_LIGHTS * sizeof(ke::PointLight), 4);

		ke::Gui gui(window);

		bool grayscale = true;
		bool hasNormaMap = true;
		bool animation = true;

		float angle = 0.f;

		float deltaTime = 0.f, lastFrame = 0.f;

		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << "\n";

		while (!window.shouldClose())
		{	
			gui.BeginFrame();

			gui.GrayScale(grayscale);
			gui.NormalMapping(hasNormaMap);
			gui.Animation(animation);

			float currTime = static_cast<float>(glfwGetTime());
			deltaTime = currTime - lastFrame;
			lastFrame = currTime;

			if (animation)
			{
				sphereTransform.position.x = cos(angle);
				sphereTransform.position.z = sin(angle);
				sphereTransform.rotation.y += (ROTATION_SPEED * deltaTime);

				angle += ANGLE_INCREASE * deltaTime;
			}

			window.pollEvents();

			for (size_t i = 0; i < shadows.size(); ++i)
			{
				shadows[i]->bind();

				ke::RenderCommand::SetViewport(shadows[i]->getWidth(), shadows[i]->getHeight());

				ke::RenderCommand::Clear(ke::ClearCommand::Depth);

				ke::RenderState shadowState = renderState;
				shadowState.cullEnabled = true;
				shadowState.cullMode = ke::CullMode::Front;

				ke::RenderCommand::ApplyRenderState(shadowState);

				shadowShader->bind();
				shadowShader->setUniformMatrix4("lightSpaceMatrix", dirLights[shadows[i]->getLightIndex()].lightSpaceMatrix);

				shadowShader->setUniformMatrix4("u_Model", transform.getModelMatrix());

				ke::RenderCommand::DrawIndexed(mesh.getVAO(), mesh.getIndexCount());

				shadowShader->setUniformMatrix4("u_Model", sphereTransform.getModelMatrix());

				ke::RenderCommand::DrawIndexed(sphereMesh.getVAO(), sphereMesh.getIndexCount());

				ke::RenderCommand::ApplyRenderState(renderState);
			}

			ke::RenderCommand::SetViewport(window.getWidth(), window.getHeight());
			
			if(grayscale)
				renderTarget.bind();
			else ke::RenderCommand::BindDefaultFramebuffer();

			ke::RenderCommand::Clear(ke::ClearCommand::Color | ke::ClearCommand::Depth);

			ke::RenderCommand::ApplyRenderState(skyboxRenderState);

			skyboxShader->bind();
			skyboxShader->setUniformMatrix4("u_VP", camera.getProjectionMatrix(window.getWidth(), window.getHeight()) * glm::mat4(glm::mat3(camera.getViewMatrix())));

			skybox->bind(ke::TextureSlot::Skybox);

			ke::RenderCommand::DrawSkybox(skybox->getVAO());

			ke::RenderCommand::ApplyRenderState(renderState);

			shader->bind();

			shader->setUniformVec3("viewPos", camera.position);
			shader->setUniformInt("dirLightsCount", (int)dirLights.size());
			shader->setUniformInt("pointLightsCount", (int)pointLights.size());

			for (size_t i = 0; i < shadows.size(); ++i)
			{
				shadows[i]->getDepthMap(assetManager, shadows[i]->getLightIndex())->bind(static_cast<ke::TextureSlot>((uint32_t)ke::TextureSlot::ShadowMap + shadows[i]->getLightIndex()));
			}

			texture->bind(ke::TextureSlot::Albedo);
			normalMap->bind(ke::TextureSlot::NormalMap);

			shader->setUniformMatrix4("u_MVP", camera.getProjectionMatrix(window.getWidth(), window.getHeight()) * camera.getViewMatrix() * transform.getModelMatrix());

			shader->setUniformMatrix4("u_Model",  transform.getModelMatrix());

			shader->setUniformMat3("u_Norm", glm::mat3(glm::transpose(glm::inverse(transform.getModelMatrix()))));
			shader->setUniformInt("normalMapping", hasNormaMap);

			ke::RenderCommand::DrawIndexed(mesh.getVAO(), mesh.getIndexCount());

			sphereTexture->bind(ke::TextureSlot::Albedo);
			sphereNormalMap->bind(ke::TextureSlot::NormalMap);

			shader->setUniformMatrix4("u_MVP", camera.getProjectionMatrix(window.getWidth(), window.getHeight()) * camera.getViewMatrix() * sphereTransform.getModelMatrix());

			shader->setUniformMatrix4("u_Model", sphereTransform.getModelMatrix());

			shader->setUniformMat3("u_Norm", glm::mat3(glm::transpose(glm::inverse(sphereTransform.getModelMatrix()))));
			shader->setUniformInt("normalMapping", hasNormaMap);

			ke::RenderCommand::DrawIndexed(sphereMesh.getVAO(), sphereMesh.getIndexCount());

			if (grayscale)
			{
				ke::RenderCommand::BindDefaultFramebuffer();

				grayScaleShader->bind();
				proceduralTexture->bind(ke::TextureSlot::GrayScale);

				ke::RenderState grayscaleRenderState{};
				grayscaleRenderState.depthTest = false;
				grayscaleRenderState.depthWrite = false;

				ke::RenderCommand::ApplyRenderState(grayscaleRenderState);

				ke::RenderCommand::Clear(ke::ClearCommand::Color);

				ke::RenderCommand::DrawFullscreenQuad();

				ke::RenderCommand::ApplyRenderState(renderState);
			}


			gui.UpdateLights(dirLights, pointLights, shadows);

			storageBuffer.uploadData(dirLights.size() * sizeof(ke::DirectionalLight), dirLights.data());
			pointStorageBuffer.uploadData(pointLights.size() * sizeof(ke::PointLight), pointLights.data());

			gui.Render();

			window.swapBuffers();
		}
	}
	
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
} 