#pragma once

#include "glWrappers/IndexBuffer.h"
#include "glWrappers/VertexArray.h"
#include "glWrappers/VertexBuffer.h"
#include "glWrappers/VertexBufferLayout.h"
#include "Material.h"
#include "Mesh.h"

namespace GLEngine
{
	class MeshRenderer
	{
	private:
		float* m_renderBuffer;
		size_t m_renderBuffSize;

		VertexBuffer* m_vbo;
		VertexArray* m_vao;
		IndexBuffer* m_ib;

		void loadGPUData(const Mesh& mesh);

	public:
		MeshRenderer(const Mesh& mesh);
		~MeshRenderer();

		void draw() const;
	};
}