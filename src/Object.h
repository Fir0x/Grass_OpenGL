#pragma once

#include <glm/glm.hpp>

#include "MeshRenderer.h"

namespace GLEngine
{
	class Object
	{
	private:
		glm::vec3 m_pos;
		MeshRenderer* m_renderer;

	public:
		Object(MeshRenderer* renderer);
		Object(const glm::vec3& pos, MeshRenderer* renderer);
		~Object();

		glm::mat4 getModelMatrix() const;

		void draw() const;
	};
}