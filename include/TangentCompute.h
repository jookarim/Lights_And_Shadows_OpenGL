#include <vector>
#include "Mesh.h"
#include <cstdint>
#include <glm/glm.hpp>

namespace ke
{
	void calculateTangent(std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			uint32_t i0 = indices[i];
			uint32_t i1 = indices[i + 1];
			uint32_t i2 = indices[i + 2];

			glm::vec3 e1 = vertices[i1].position - vertices[i0].position;
			glm::vec3 e2 = vertices[i2].position - vertices[i0].position;

			glm::vec2 deltaUV1 = vertices[i1].texCoord - vertices[i0].texCoord;
			glm::vec2 deltaUV2 = vertices[i2].texCoord - vertices[i0].texCoord;

			float det = deltaUV1.x * deltaUV2.y + deltaUV2.x * deltaUV1.y;

			float f = 1.f / det;

			glm::vec3 tangent = glm::normalize(f * (deltaUV2.y * e1 - deltaUV1.y * e2));

			vertices[i0].tangent += tangent;
			vertices[i1].tangent += tangent;
			vertices[i2].tangent += tangent;
		}
	}
}