#include <iostream>
#include "Window.h"
#include "Mesh.h"
#include "RenderCommand.h"

int main()
{
	try
	{
		ke::WindowDesc windowDesc {
			.width = 1270,
			.height = 720,
			.title = "Window"
		};

		ke::Window window(std::move(windowDesc), {});

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

		while (!window.shouldClose())
		{
			window.pollEvents();

			ke::RenderCommand::Clear(ClearCommand::Color | ClearCommand::Depth);

			ke::RenderCommand::DrawIndexed(mesh.getVAO(), mesh.getIndexCount());

			window.swapBuffers();
		}
	} 
	
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	
}