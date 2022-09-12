#pragma once

namespace GLEngine
{
	class IndexBuffer
	{
	private:
		unsigned int m_id;
		int m_count;

	public:
		IndexBuffer(const unsigned int *indices, int count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		int count() const;
	};
}