#include "MeshRenderer.h"

#include "glWrappers/glError.h"

namespace GLEngine
{
	void MeshRenderer::loadGPUData(const StaticMesh& mesh)
	{
		const auto& verticesData = mesh.getData();
		m_vbo = new TypedBuffer<StaticMesh::Vertex>(verticesData.data(), verticesData.size());

		VertexBufferLayout layout;
		layout.Add<float>(3);
		layout.Add<float>(3);
		layout.Add<float>(2);

		m_vao = new VertexArray(*m_vbo, layout);
		auto& indices = mesh.getIndices();
		m_ibo = new TypedBuffer(indices.data(), indices.size());
	}

	MeshRenderer::MeshRenderer(const StaticMesh& mesh)
	{
		loadGPUData(mesh);
		m_materialId = MaterialLibrary::getDefaultId();
	}

	MeshRenderer::~MeshRenderer()
	{
		delete m_ibo;
		delete m_vao;
		delete m_vbo;
	}

	void MeshRenderer::setMaterial(unsigned int id)
	{
		m_materialId = id;
	}

	void MeshRenderer::draw(const DrawContext& context) const
	{
		MaterialLibrary::useMaterial(m_materialId, context.currentShader);
		m_vao->bind();
		m_ibo->bind(ByteBuffer::UsageType::IndexBuffer);
		GL_CALL(glDrawElements(GL_TRIANGLES, m_ibo->getCount(), GL_UNSIGNED_INT, nullptr));
	}
}