#pragma once

#include "ByteBuffer.h"
#include "VertexBufferLayout.h"

namespace GLEngine
{
	class VertexArray
	{
	private:
		unsigned int m_id;
		
	public:
		VertexArray(const ByteBuffer& vb, const VertexBufferLayout& layout);
		VertexArray(const VertexArray&) = delete;
		~VertexArray();

		void bind() const;
		void unbind() const;
	};
}