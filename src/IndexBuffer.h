#pragma once

namespace GLEngine
{
	class IndexBuffer
	{
	private:
		unsigned int m_id;

	public:
		IndexBuffer(unsigned int *indices, size_t count);
		~IndexBuffer();

		void bind();
		void unbind();
	};
}