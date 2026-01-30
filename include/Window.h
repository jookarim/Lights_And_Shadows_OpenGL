#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace ke
{
	//Window desc init with default values
	struct WindowDesc
	{
		int width = 800, height = 600;
		std::string title = "";
	};

	//Handles opengl context data like version
	struct ContextDesc
	{
		int majorVersion{ 4 };
		int minorVersion{ 5 };
		int glProfile = GLFW_OPENGL_CORE_PROFILE;
	};

	//window class for creating and handling glfw Window and opengl context
	class Window
	{
	private:
		WindowDesc m_wndDesc{};
		ContextDesc m_ctxDesc{};

		GLFWwindow* m_handle = nullptr;
	private:
		void createWindow(const WindowDesc& wndDesc, const ContextDesc& ctxDesc);
		void destroyWindow() noexcept;
	public:
		Window(const WindowDesc& desc, const ContextDesc& ctxDesc);
		~Window() noexcept;

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Window(Window&&) = delete;
		Window& operator=(Window&&) = delete;

		bool shouldClose() const noexcept;

		void swapBuffers() const noexcept;
		void pollEvents() const noexcept;

		const WindowDesc& getWindowDesc() const { return m_wndDesc; }
		const ContextDesc& getContextDesc() const { return m_ctxDesc; }

		int getWidth() const { return m_wndDesc.width; }
		int getHeight() const { return m_wndDesc.height; }
	};
}
