#pragma once

#include <iostream>
#include <glm/glm.hpp>

namespace ke
{
	struct Light
	{
		glm::vec3 ambient{};
		glm::vec3 diffuse{};
		glm::vec3 specular{};
	};

	struct DirectionalLight : public Light
	{
		glm::vec3 direction{};
	};
}