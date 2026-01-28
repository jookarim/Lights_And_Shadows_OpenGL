#include <iostream>
#include "Window.h"
#include "Mesh.h"
#include "RenderCommand.h"
#include "Shader.h"

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

		ke::Shader shader(
			{
				"assets/shaders/shader.vert",
				"assets/shaders/shader.frag" 
			}
		);

		while (!window.shouldClose())
		{
			window.pollEvents();

			ke::RenderCommand::Clear(ke::ClearCommand::Color | ke::ClearCommand::Depth);

			shader.bind();

			ke::RenderCommand::DrawIndexed(mesh.getVAO(), mesh.getIndexCount());

			window.swapBuffers();
		}
	} 
	
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	
}