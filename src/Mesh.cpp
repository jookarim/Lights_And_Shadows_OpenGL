#include "Mesh.h"

namespace ke
{
	void Mesh::createBuffers(const MeshData& meshData)
	{
		m_indexCount = meshData.indices.size();

		glCreateVertexArrays(1, &m_vao);
		glCreateBuffers(1, &m_vbo);
		glCreateBuffers(1, &m_ibo);

		glNamedBufferData(m_vbo, sizeof(Vertex) * meshData.vertices.size(), meshData.vertices.data(), GL_STATIC_DRAW);
		glNamedBufferData(m_ibo, sizeof(uint32_t) * meshData.indices.size(), meshData.indices.data(), GL_STATIC_DRAW);

		glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));
		glVertexArrayElementBuffer(m_vao, m_ibo);

		glEnableVertexArrayAttrib(m_vao, 0);
		glVertexArrayAttribBinding(m_vao, 0, 0);
		glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));

		glEnableVertexArrayAttrib(m_vao, 1);
		glVertexArrayAttribBinding(m_vao, 1, 0);
		glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texCoord));

		glEnableVertexArrayAttrib(m_vao, 2);
		glVertexArrayAttribBinding(m_vao, 2, 0);
		glVertexArrayAttribFormat(m_vao, 2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	}
	
	void Mesh::destroyBuffers() noexcept
	{
		if (m_vao)
		{
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}

		if (m_vbo)
		{
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}

		if (m_ibo)
		{
			glDeleteBuffers(1, &m_ibo);
			m_ibo = 0;
		}
	}

	Mesh::Mesh(const MeshData& meshData)
	{
		createBuffers(meshData);
	}

	Mesh::~Mesh() noexcept
	{
		destroyBuffers();
	}

	Mesh::Mesh(Mesh&& other) noexcept
	{
		m_vao = other.m_vao;
		m_vbo = other.m_vbo;
		m_ibo = other.m_ibo;
		m_indexCount = other.m_indexCount;

		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ibo = 0;
		other.m_indexCount = 0;
	}

	Mesh& Mesh::operator=(Mesh&& other) noexcept
	{
		if (this == &other) return *this;

		destroyBuffers();

		m_vao = other.m_vao;
		m_vbo = other.m_vbo;
		m_ibo = other.m_ibo;
		m_indexCount = other.m_indexCount;

		other.m_vao = 0;
		other.m_vbo = 0;
		other.m_ibo = 0;
		other.m_indexCount = 0;

		return *this;
	}

	void Mesh::bind() const
	{
		glBindVertexArray(m_vao);
	}
}