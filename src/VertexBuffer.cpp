#include "VertexBuffer.h"

#include <GL/glew.h>

#include "glError.h"

namespace GLEngine
{
	VertexBuffer::VertexBuffer(const void* buffer, size_t size)
	{
		GL_CALL(glGenBuffers(1, &m_id));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW));
	}

	VertexBuffer::~VertexBuffer()
	{
		GL_CALL(glDeleteBuffers(1, &m_id));
	}

	void VertexBuffer::bind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
	}

	void VertexBuffer::unbind() const
	{
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}