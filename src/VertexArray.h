#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace GLEngine
{
	class VertexArray
	{
	private:
		unsigned int m_id;
		
	public:
		VertexArray(const VertexBuffer& vb, const VertexBufferLayout& layout);
		~VertexArray();

		void bind() const;
		void unbind() const;
	};
}