#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cstdint>
#include <cstddef>

namespace ke
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};

	struct MeshData
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
	};

	class Mesh
	{
	private:
		GLuint m_vao{};
		GLuint m_vbo{};
		GLuint m_ibo{};
		uint32_t m_indexCount{};

	private:
		void createBuffers(const MeshData& meshData);

		void destroyBuffers() noexcept;

	public:
		Mesh(const MeshData& meshData);
		~Mesh() noexcept;

		Mesh(const Mesh&) = delete;
		Mesh& operator=(const Mesh&) = delete;

		Mesh(Mesh&& other) noexcept;
		Mesh& operator=(Mesh&& other) noexcept;

		void bind() const;

		uint32_t getIndexCount() const { return m_indexCount; }
	};
}