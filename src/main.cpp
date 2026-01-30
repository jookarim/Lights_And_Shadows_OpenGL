#include <iostream>
#include "Window.h"
#include "Mesh.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Texture.h"
#include "AssetManager.h"
#include "Transform.h"

int main()
{
	try
	{
		ke::WindowDesc windowDesc {
			.width = 1270,
			.height = 720,
			.title = "Window"
		};

		ke::Window window(windowDesc, {});

		ke::MeshData meshData{
			.vertices = {
				{{0.f, 1.f, 0.f}, {0.f, 1.f}, {0.f, 0.f, 1.f}},
				{{1.f, -1.f, 0.f}, {1.f, 0.f}, {0.f, 0.f, 1.f}},
				{{-1.f, -1.f, 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f}}
			},

			.indices = {
				0,1,2
			}
		};

		ke::Mesh mesh(meshData);

		ke::RenderCommand::ClearColor(1.f, 1.f, 0.f, 1.f);

		ke::ShaderDesc shaderDesc{
			.vertPath = "assets/shaders/shader.vert",
			.fragPath = "assets/shaders/shader.frag"
		};

		auto& assetManager = ke::AssetManager::getInstance();

		auto shader = assetManager.loadShader("shader", shaderDesc);
		auto texture = assetManager.loadTexture("texture", "assets/images/brick.png");

		ke::Transform transform{
			.position{0.f, 1.f, 0.f},
			.rotation{0.f, 0.f, 45.f},
			.scale{0.5f}
		};

		while (!window.shouldClose())
		{
			window.pollEvents();

			ke::RenderCommand::Clear(ke::ClearCommand::Color | ke::ClearCommand::Depth);

			shader->bind();

			shader->setUniformMatrix4("u_Model", transform.getModelMatrix());

			texture->bind(0);
			shader->setUniformInt("albedo", 0);
			
			ke::RenderCommand::DrawIndexed(mesh.getVAO(), mesh.getIndexCount());

			window.swapBuffers();
		}
	} 
	
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	
}