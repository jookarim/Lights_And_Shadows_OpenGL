#include "Gui.h"

namespace ke
{
	void Gui::Init(const ke::Window& window)
	{
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window.getHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");
	}

	void Gui::Shutdown() noexcept
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	Gui::Gui(const ke::Window& window)
	{
		Init(window);
	}

	Gui::~Gui() noexcept
	{
		Shutdown();
	}

	void Gui::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Gui::Render()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Gui::NormalMapping(bool& hasNormalMapping)
	{
		ImGui::Checkbox("Normal Mapping", &hasNormalMapping);
	}

	void Gui::GrayScale(bool& grayScale)
	{
		ImGui::Checkbox("Grayscale", &grayScale);
	}

	void Gui::UpdateLights(std::vector<DirectionalLight>& dirLights, std::vector<PointLight>& pointLights, std::vector<std::unique_ptr<DirectionalShadow>>& dirShadows)
	{
		if (ImGui::CollapsingHeader("Point Lights"))
		{
			for (size_t i = 0; i < pointLights.size(); ++i)
			{
				ImGui::PushID((int)i + 1);

				ImGui::Text("Point Light %d", (int)i);
				ImGui::SliderFloat3("Position", &pointLights[i].position.x, -30.f, 30.f);
				ImGui::ColorEdit3("Ambient", &pointLights[i].ambient.x);
				ImGui::ColorEdit3("Diffuse", &pointLights[i].diffuse.x);
				ImGui::ColorEdit3("Specular", &pointLights[i].specular.x);

				ImGui::Separator();
				ImGui::PopID();
			}
		}

		if (ImGui::CollapsingHeader("Directional Lights"))
		{
			for (size_t i = 0; i < dirLights.size(); ++i)
			{
				ImGui::PushID((int)i + 2);

				ImGui::Text("Directional Light %d", (int)i);
				ImGui::SliderFloat3("Direction", &dirLights[i].direction.x, -3.f, 3.f);
				ImGui::ColorEdit3("Ambient", &dirLights[i].ambient.x);
				ImGui::ColorEdit3("Diffuse", &dirLights[i].diffuse.x);
				ImGui::ColorEdit3("Specular", &dirLights[i].specular.x);

				dirShadows[i]->updateFromLight(dirLights[i]);
				dirLights[i].lightSpaceMatrix = dirShadows[i]->getLightSpaceMatrix();

				ImGui::Separator();
				ImGui::PopID();
			}
		}
	}

	void Gui::Animation(bool& animation)
	{
		ImGui::Checkbox("Animation", &animation);
	}

	void Gui::Rotation(bool& rotation)
	{
		ImGui::Checkbox("Rotation", &rotation);
	}

	void Gui::sliderPCF(int& pcfValue)
	{
		int exponent = pcfValue * pcfValue;

		if (ImGui::SliderInt("PCF Samples", &exponent, 1, 81))
		{
			int kernel = static_cast<int>(std::sqrt(exponent));
			
			constexpr int maxSamples = 81;
			int maxKernel = static_cast<int>(std::sqrt(maxSamples));
			kernel = std::min(kernel, maxKernel);
			
			if (kernel % 2 == 0)
				kernel += 1;
			
			kernel = std::min(kernel, 9);

			pcfValue = kernel;
		}

		std::string samplesText = "PCF Samples " + std::to_string((int)pcfValue) + "x" + std::to_string((int)pcfValue);

		ImGui::Text(samplesText.c_str()); 
	}

	void Gui::PCF(bool& pcfEnabled)
	{
		ImGui::Checkbox("Enable PCF", &pcfEnabled);
	}
}
