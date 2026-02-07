#include <vector>
#include "Mesh.h"

namespace ke
{
	MeshData generateSphere()
	{
		MeshData sphereMeshData{};

		const float sphereRadius = 5.0f;
		const uint32_t latitudeCount = 32;
		const uint32_t longitudeCount = 32;

		for (uint32_t lat = 0; lat <= latitudeCount; ++lat)
		{
			float v = (float)lat / latitudeCount;
			float polarAngle = v * glm::pi<float>();

			for (uint32_t lon = 0; lon <= longitudeCount; ++lon)
			{
				float u = (float)lon / longitudeCount;
				float azimuthAngle = u * glm::two_pi<float>();

				float x = sphereRadius * sin(polarAngle) * cos(azimuthAngle);
				float y = sphereRadius * cos(polarAngle);
				float z = sphereRadius * sin(polarAngle) * sin(azimuthAngle);

				glm::vec3 position{ x, y, z };
				glm::vec3 normal = glm::normalize(position);
				glm::vec2 texCoord{ u, 1.0f - v };

				sphereMeshData.vertices.push_back({position,texCoord,normal});
			}
		}

		for (uint32_t lat = 0; lat < latitudeCount; ++lat)
		{
			for (uint32_t lon = 0; lon < longitudeCount; ++lon)
			{
				uint32_t topLeft = lat * (longitudeCount + 1) + lon;
				uint32_t bottomLeft = topLeft + longitudeCount + 1;

				sphereMeshData.indices.push_back(topLeft);
				sphereMeshData.indices.push_back(bottomLeft);
				sphereMeshData.indices.push_back(topLeft + 1);

				sphereMeshData.indices.push_back(bottomLeft);
				sphereMeshData.indices.push_back(bottomLeft + 1);
				sphereMeshData.indices.push_back(topLeft + 1);
			}
		}

		ke::calculateTangent(sphereMeshData.vertices, sphereMeshData.indices);

		return sphereMeshData;
	}
}