#include "ByteBuffer.h"

#include <GL/glew.h>

#include "glError.h"

#include <iostream>

namespace GLEngine
{
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

	static GLenum usageType2GL(ByteBuffer::UsageType usageType)
	{
		switch (usageType)
		{
		case ByteBuffer::VertexBuffer:
			return GL_ARRAY_BUFFER;
		case ByteBuffer::IndexBuffer:
			return GL_ELEMENT_ARRAY_BUFFER;
		}
	}

	static GLenum accessType2GL(ByteBuffer::AccessType accessType)
	{
		if (accessType == ByteBuffer::AccessType::Read)
			return GL_READ_ONLY;
		else if (accessType == ByteBuffer::AccessType::Write)
			return GL_WRITE_ONLY;
		else
			return GL_READ_WRITE;
	}

	void ByteBuffer::bind(UsageType usageType) const
	{
		GL_CALL(glBindBuffer(usageType2GL(usageType), m_id));
	}

	void ByteBuffer::setAccess(AccessType accessType) const
	{
		GL_CALL(glMapNamedBuffer(m_id, accessType2GL(accessType)));
	}
}