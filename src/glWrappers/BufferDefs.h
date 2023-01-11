#pragma once

#include <GL/glew.h>

namespace GLEngine
{
	enum BufferUsageType
	{
		VertexBuffer,
		IndexBuffer,
		UniformBuffer,
		ShaderStorage,
	};

	enum BufferAccessType
	{
		Read,
		Write,
		Read_Write
	};

	GLenum usageType2GL(BufferUsageType usageType);
	GLenum accessType2GL(BufferAccessType accessType);
}