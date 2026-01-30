#include "Camera.h"

namespace ke
{
	glm::mat4 Camera::getViewMatrix() const
	{
		return glm::lookAt(position, position + forward, up);
	}

	glm::mat4 Camera::getProjectionMatrix(int windowWidth, int windowHeight) const
	{
		return glm::perspective(glm::radians(fov), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), near, far);
	}
}
