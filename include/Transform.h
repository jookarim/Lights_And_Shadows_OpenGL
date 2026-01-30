#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ke
{
	struct Transform
	{
		glm::vec3 position{};
		glm::vec3 rotation{};
		glm::vec3 scale{1.f};

		glm::mat4 getModelMatrix() const;
	};
}