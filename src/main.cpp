#include <iostream>
#include "Window.h"

int main()
{
	try
	{
		ke::WindowDesc windowDesc = {
			.width = 1270,
			.height = 720,
			.title = "Window"
		};

		ke::Window window(std::move(windowDesc), {});

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