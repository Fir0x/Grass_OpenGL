#include "Object.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

#include "glWrappers/glError.h"

namespace GLEngine
{
	Object::Object(glm::vec3 pos, const Mesh& mesh, const Material* material)
	{
		m_pos = pos;
		m_material = material;
		initializeRender(mesh);
	}

	Object::Object(const Mesh& mesh)
	{
		m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
		initializeRender(mesh);
	}

	void Object::initializeRender(const Mesh& mesh)
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

	Object::~Object()
	{
		free(m_renderBuffer);
		delete m_ib;
		delete m_vao;
		delete m_vbo;
	}

	glm::mat4 Object::getModelMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), m_pos);
	}

	void Object::draw() const
	{
		m_vao->bind();
		m_ib->bind();
		GL_CALL(glDrawElements(GL_TRIANGLES, m_ib->count(), GL_UNSIGNED_INT, nullptr));
	}
}