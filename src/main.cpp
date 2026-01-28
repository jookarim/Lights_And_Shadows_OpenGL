#include <iostream>
#include "Window.h"
#include "Mesh.h"

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

		while (!window.shouldClose())
		{
			window.pollEvents();

			window.swapBuffers();
		}
	} 
	
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	
}