#pragma once

#include "glWrappers/TypedBuffer.h"
#include "glWrappers/VertexArray.h"
#include "glWrappers/VertexBufferLayout.h"
#include "scene/IDrawable.h"
#include "render/Material.h"
#include "render/StaticMesh.h"

namespace GLEngine
{
	class MeshRenderer : IDrawable
	{
	private:
		float* m_renderBuffer;
		size_t m_renderBuffSize;

		ByteBuffer* m_vbo;
		VertexArray* m_vao;
		TypedBuffer<unsigned int>* m_ibo;

		unsigned int m_materialId;

		void loadGPUData(const StaticMesh& mesh);

	public:
		MeshRenderer(const StaticMesh& mesh);
		~MeshRenderer();

		void setMaterial(unsigned int id);

		virtual void draw(const DrawContext& context) const override;
	};
}