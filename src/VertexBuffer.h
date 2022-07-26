#pragma once

namespace GLEngine
{
	class VertexBuffer
	{
	private:
		unsigned int m_id;

	public:
		VertexBuffer(const void *buffer, size_t size);
		~VertexBuffer();

		void bind() const;
		void unbind() const;
	};
}