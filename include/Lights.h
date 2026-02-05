#pragma once

#include <iostream>
#include <glm/glm.hpp>

namespace ke
{
	struct DirectionalLight
	{
		glm::vec3 ambient{};
		float pad0;
		glm::vec3 diffuse{};
		float pad1;
		glm::vec3 specular{};
		float pad2;
		glm::vec3 direction{};
		float pad3;
	};

	struct PointLight
	{
		glm::vec3 ambient{};
		float pad0;

		glm::vec3 diffuse{};
		float pad1;

		glm::vec3 specular{};
		float pad2;

		glm::vec3 position{};
		float constant;

		float linear;
		float quadric;
		float pad3;
	};

}