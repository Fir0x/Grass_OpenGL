#pragma once

namespace GLEngine
{
	class IndexBuffer
	{
	private:
		unsigned int m_id;
		size_t m_count;

	public:
		IndexBuffer(const unsigned int *indices, size_t count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		size_t count() const;
	};
}