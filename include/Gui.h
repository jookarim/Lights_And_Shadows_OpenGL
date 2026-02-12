#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends//imgui_impl_glfw.h>
#include "Lights.h"
#include "Window.h"
#include "DirectionalShadow.h"
#include <memory>

namespace ke
{
	class Gui
	{
	private:
		void Init(const ke::Window& window);
		void Shutdown() noexcept;
		void AddDirectionalLight(std::vector<DirectionalLight>& dirLights, std::vector<std::unique_ptr<DirectionalShadow>>& dirShadows);
		void AddPointLight(std::vector<PointLight>& pointLights);
	public:
		Gui(const ke::Window& window);
		~Gui() noexcept;

		void UpdateLights(std::vector<DirectionalLight>& dirLights, std::vector<PointLight>& pointLights, std::vector<std::unique_ptr<DirectionalShadow>>& dirShadows);
		void NormalMapping(bool& hasNormalMapping);
		void GrayScale(bool& grayScale);
		void Animation(bool& animation);
		void Rotation(bool& rotation);
		void sliderPCF(int& pcfValue, bool& ai);
		void PCF(bool& pcfEnabled);
		void PrintFPS();
		
		void BeginFrame();
		void Render();
	};
}