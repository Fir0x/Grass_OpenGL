#pragma once

#include "BufferDefs.h"

namespace GLEngine
{
	class ByteBuffer
	{
	private:
		unsigned int m_id;

	protected:
		size_t m_size;

	public:
		ByteBuffer(size_t size);
		ByteBuffer(const void* data, size_t size);
		~ByteBuffer();

		void bind(BufferUsageType usageType) const;
		/// <summary>
		/// Bind a buffer as a uniform buffer on a binding point..
		/// </summary>
		/// <param name="bindingPoint"></param>
		
		template<BufferUsageType U>
		void bind(int bindingPoint) const
		{
			static_assert(U == BufferUsageType::UniformBuffer || U == BufferUsageType::ShaderStorage, "Bad usage type");
			glBindBufferBase(usageType2GL(U), bindingPoint, m_id);
		}

		void setAccess(BufferAccessType accessType) const;
	};
}

