#pragma once

#include <glm/glm.hpp>

#include "Material.h"
#include "Mesh.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace GLEngine
{
	class Object
	{
	private:
		float* m_renderBuffer;
		size_t m_renderBuffSize;
		glm::vec3 m_pos;
		const Material* m_material;

		VertexBuffer* m_vbo;
		VertexArray* m_vao;
		IndexBuffer* m_ib;

		void initializeRender(const Mesh &mesh);

	public:
		Object(glm::vec3 pos, const Mesh &mesh, const Material* material);
		Object(const Mesh& mesh);
		~Object();

		glm::mat4 getModelMatrix() const;

		void draw() const;
	};
}