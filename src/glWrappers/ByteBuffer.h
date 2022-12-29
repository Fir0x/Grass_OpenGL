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
			IndexBuffer,
			UniformBuffer
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
		/// <summary>
		/// Bind a buffer as a uniform buffer on a binding point..
		/// </summary>
		/// <param name="bindingPoint"></param>
		void bind(int bindingPoint) const;
		void setAccess(AccessType accessType) const;
	};
}

