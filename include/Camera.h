#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ke
{
	class Camera
	{
	public:
		float near = 1.f;
		float far = 1000.f;
		glm::vec3 position{ 0.f, 0.f, 3.f };
		glm::vec3 forward{ 0.f, 0.f, -1.f };
		glm::vec3 up = { 0.f, 1.f, 0.f };
		float fov = 45.f;

		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjectionMatrix(int windowWidth, int windowHeight) const;
	};
}