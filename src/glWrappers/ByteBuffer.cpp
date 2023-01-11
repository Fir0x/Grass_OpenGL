#include "ByteBuffer.h"

#include "glError.h"

#include <iostream>

namespace GLEngine
{
	ByteBuffer::ByteBuffer(size_t size)
	{
		m_size = size;
		GL_CALL(glCreateBuffers(1, &m_id));
		GL_CALL(glNamedBufferData(m_id, size, nullptr, GL_STATIC_DRAW));
	}

	ByteBuffer::ByteBuffer(const void* data, size_t size)
	{
		m_size = size;
		GL_CALL(glCreateBuffers(1, &m_id));
		GL_CALL(glNamedBufferData(m_id, size, data, GL_STATIC_DRAW));
	}

	ByteBuffer::~ByteBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}

	void ByteBuffer::bind(BufferUsageType usageType) const
	{
		GL_CALL(glBindBuffer(usageType2GL(usageType), m_id));
	}

	void ByteBuffer::setAccess(BufferAccessType accessType) const
	{
		GL_CALL(glMapNamedBuffer(m_id, accessType2GL(accessType)));
	}
}