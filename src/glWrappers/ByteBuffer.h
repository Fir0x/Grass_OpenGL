#pragma once

namespace GLEngine
{
	class ByteBuffer
	{
	private:
		unsigned int m_id;

	protected:
		size_t m_size;

	public:
		enum UsageType
		{
			VertexBuffer,
			IndexBuffer
		};

		enum AccessType
		{
			Read,
			Write,
			Read_Write
		};

		ByteBuffer(const void* data, size_t size);
		~ByteBuffer();

		void bind(UsageType usageType) const;
		void setAccess(AccessType accessType) const;
	};
}

