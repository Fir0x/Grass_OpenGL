#include "IndexBuffer.h"

#include <GL/glew.h>

#include "glError.h"

namespace GLEngine
{
	IndexBuffer::IndexBuffer(const unsigned int* indices, int count)
	{
		m_count = count;

		GL_CALL(glGenBuffers(1, &m_id));
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
		GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GL_CALL(glDeleteBuffers(1, &m_id));
	}

	void IndexBuffer::bind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
	}

	void IndexBuffer::unbind() const
	{
		GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}

	int IndexBuffer::count() const
	{
		return m_count;
	}
}