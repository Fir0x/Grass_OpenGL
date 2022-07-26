#pragma once

#include "VertexBuffer.h"

namespace GLEngine
{
	class VertexArray
	{
	private:
		unsigned int m_id;
		
	public:
		VertexArray(const VertexBuffer& vb);
		~VertexArray();

		void bind() const;
		void unbind() const;
	};
}