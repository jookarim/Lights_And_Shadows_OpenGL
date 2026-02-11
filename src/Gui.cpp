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
		AddPointLight(pointLights);

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

		AddDirectionalLight(dirLights, dirShadows);

		if (ImGui::CollapsingHeader("Directional Lights"))
		{
			for (size_t i = 0; i < dirLights.size(); ++i)
			{
				ImGui::PushID((int)i);

				dirShadows[i]->updateFromLight(dirLights[i]);
				dirLights[i].lightSpaceMatrix = dirShadows[i]->getLightSpaceMatrix();

				ImGui::Text("Directional Light %d", (int)i);
				ImGui::SliderFloat3("Direction", &dirLights[i].direction.x, -3.f, 3.f);
				ImGui::ColorEdit3("Ambient", &dirLights[i].ambient.x);
				ImGui::ColorEdit3("Diffuse", &dirLights[i].diffuse.x);
				ImGui::ColorEdit3("Specular", &dirLights[i].specular.x);

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
		if (ImGui::SliderInt("PCF Samples", &pcfValue, 1, 19))
		{

		}

		std::string samplesText = "PCF Samples " + std::to_string((int)pcfValue) + "x" + std::to_string((int)pcfValue);

		ImGui::Text(samplesText.c_str());
	}

	void Gui::PCF(bool& pcfEnabled)
	{
		ImGui::Checkbox("Enable PCF", &pcfEnabled);
	}

	void Gui::PrintFPS()
	{
		ImGuiIO& io = ImGui::GetIO();

		std::string fpsTxt = "FPS: " + std::to_string(io.Framerate);

		ImGui::Text("%s", fpsTxt.c_str());
	}

	void Gui::AddDirectionalLight(std::vector<DirectionalLight>& dirLights, std::vector<std::unique_ptr<DirectionalShadow>>& dirShadows)
	{
		if (ImGui::Button("Add Directional Light"))
		{
			DirectionalLight newLight{};

			newLight.ambient = glm::vec3(0.08f);
			newLight.diffuse = glm::vec3(1.f);
			newLight.specular = glm::vec3(1.f);
			newLight.direction = glm::normalize(glm::vec3(-1.f, -1.f, -1.f));

			dirLights.push_back(newLight);

			int index = static_cast<int>(dirLights.size()) - 1;

			dirShadows.push_back(std::make_unique<DirectionalShadow>(glm::ivec2(2048, 2048), AssetManager::getInstance(), dirLights[index], index));
		}
	}

	void Gui::AddPointLight(std::vector<PointLight>& pointLights)
	{
		if (ImGui::Button("Add Point Light"))
		{
			PointLight newLight{};
			newLight.ambient = glm::vec3(0.08f);
			newLight.diffuse = glm::vec3(1.f);
			newLight.specular = glm::vec3(1.f);
			newLight.position = glm::vec3(0.f, 5.f, 0.f);
			newLight.constant = 1.f;
			newLight.linear = 0.09f;
			newLight.quadric = 0.032f;
			pointLights.push_back(newLight);
		}
	}

}
