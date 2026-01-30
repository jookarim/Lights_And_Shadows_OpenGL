#include "Window.h"
#include <stdexcept>

namespace ke
{
	void Window::createWindow(const WindowDesc& wndDesc, const ContextDesc& ctxDesc)
	{
		m_wndDesc = wndDesc;
		m_ctxDesc = ctxDesc;

		if (!glfwInit())
		{
			glfwTerminate();
			throw std::runtime_error("Failed to init glfw");
		}

		glfwWindowHint(GLFW_OPENGL_PROFILE, ctxDesc.glProfile);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, ctxDesc.majorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, ctxDesc.minorVersion);

		m_handle = glfwCreateWindow(wndDesc.width, wndDesc.height, wndDesc.title.c_str(), nullptr, nullptr);

		if (!m_handle)
		{
			glfwTerminate();
			throw std::runtime_error("Failed to create window");
		}

		glfwMakeContextCurrent(m_handle);

		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
		{
			destroyWindow();
			throw std::runtime_error("Failed to create opengl context");
		}

		glViewport(0, 0, m_wndDesc.width, m_wndDesc.height);
	}

	void Window::destroyWindow() noexcept
	{
		if (m_handle)
		{
			glfwDestroyWindow(m_handle);
			m_handle = nullptr;
		}
	}

	Window::Window(const WindowDesc& wndDesc, const ContextDesc& ctxDesc)
	{
		createWindow(wndDesc, ctxDesc);
	}

	Window::~Window() noexcept
	{
		destroyWindow();
		glfwTerminate();
	}

	bool Window::shouldClose() const noexcept
	{
		return glfwWindowShouldClose(m_handle);
	}

	void Window::swapBuffers() const noexcept
	{
		glfwSwapBuffers(m_handle);
	}

	void Window::pollEvents() const noexcept
	{
		glfwPollEvents();
	}
}