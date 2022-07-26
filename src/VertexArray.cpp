#include "VertexArray.h"

#include <GL/glew.h>

#include "glError.h"

namespace GLEngine
{
	VertexArray::VertexArray(const VertexBuffer& vb)
	{
		GL_CALL(glGenVertexArrays(1, &m_id));
		GL_CALL(glBindVertexArray(m_id));

		vb.bind();
		GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr));
		GL_CALL(glEnableVertexAttribArray(0));
	}

	VertexArray::~VertexArray()
	{
		GL_CALL(glDeleteVertexArrays(1, &m_id));
	}

	void VertexArray::bind() const
	{
		GL_CALL(glBindVertexArray(m_id));
	}

	void VertexArray::unbind() const
	{
		GL_CALL(glBindVertexArray(0));
	}
}