#include "MeshRenderer.h"

#include "glWrappers/glError.h"

namespace GLEngine
{
	void MeshRenderer::loadGPUData(const Mesh& mesh)
	{
		m_renderBuffer = mesh.generateBuffer(m_renderBuffSize);
		m_vbo = new VertexBuffer(m_renderBuffer, m_renderBuffSize);
		VertexBufferLayout layout;
		layout.Add<float>(3);
		layout.Add<float>(3);
		layout.Add<float>(2);

		m_vao = new VertexArray(*m_vbo, layout);
		auto& indices = mesh.getIndices();
		m_ib = new IndexBuffer(indices.data(), (int)indices.size());
	}

	MeshRenderer::MeshRenderer(const Mesh& mesh)
	{
		loadGPUData(mesh);
		m_materialId = MaterialLibrary::getDefaultId();
	}

	MeshRenderer::~MeshRenderer()
	{
		free(m_renderBuffer);
		delete m_ib;
		delete m_vao;
		delete m_vbo;
	}

	void MeshRenderer::setMaterial(unsigned int id)
	{
		m_materialId = id;
	}

	void MeshRenderer::draw(Shader& shader) const
	{
		MaterialLibrary::useMaterial(m_materialId, shader);
		m_vao->bind();
		m_ib->bind();
		GL_CALL(glDrawElements(GL_TRIANGLES, m_ib->count(), GL_UNSIGNED_INT, nullptr));
	}
}