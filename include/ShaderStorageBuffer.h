#pragma once
#include <GL/glew.h>

namespace ke
{
	class ShaderStorageBuffer
	{
	private:
		GLuint m_id{};
		int m_size{};

		void createSSBO(int size, int bindingPoint);
		void destroySSBO() noexcept;
	public:
		ShaderStorageBuffer(int size ,int bindingPoint);
		~ShaderStorageBuffer() noexcept;

		void uploadData(int size, const void* data);
	};
}