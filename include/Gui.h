#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends//imgui_impl_glfw.h>
#include "Lights.h"
#include "Window.h"
#include "DirectionalShadow.h"

namespace ke
{
	class Gui
	{
	private:
		void Init(const ke::Window& window);
		void Shutdown() noexcept;
	public:
		Gui(const ke::Window& window);
		~Gui() noexcept;

		void UpdateLights(std::vector<DirectionalLight>& dirLights, std::vector<PointLight>& pointLights, std::vector<std::unique_ptr<DirectionalShadow>>& dirShadows);
		void NormalMapping(bool& hasNormalMapping);
		void GrayScale(bool& grayScale);
		void Animation(bool& animation);
		void Rotation(bool& rotation);
		void Draw(std::vector<DirectionalLight>& dirLights, std::vector<PointLight>& pointLights, std::vector<std::unique_ptr<DirectionalShadow>>& dirShadows, bool& grayscale, bool& normalMapping);

		void BeginFrame();
		void Render();
	};
}