#include "ShaderStorageBuffer.h"

namespace ke
{
	void ShaderStorageBuffer::createSSBO(int size, int bindingPoint)
	{
		glCreateBuffers(1, &m_id);
		glNamedBufferStorage(m_id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_id);
	}

	void ShaderStorageBuffer::destroySSBO() noexcept
	{
		if (m_id)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

	ShaderStorageBuffer::ShaderStorageBuffer(int size, int bindingPoint)
	{
		createSSBO(size, bindingPoint);
	}

	ShaderStorageBuffer::~ShaderStorageBuffer() noexcept
	{
		destroySSBO();
	}

	void ShaderStorageBuffer::uploadData(int size, const void* data)
	{
		glNamedBufferSubData(m_id, 0, size, data);
	}

	void ShaderStorageBuffer::bindBase(int bindingPoint) const noexcept
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_id);
	}
}