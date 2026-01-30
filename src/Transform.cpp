#include "Transform.h"

namespace ke
{
	glm::mat4 Transform::getModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.f);

		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, scale);

		return model;
	}
}