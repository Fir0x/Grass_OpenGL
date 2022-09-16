#include "Object.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

#include "glWrappers/glError.h"

namespace GLEngine
{
	Object::Object(MeshRenderer* renderer)
	{
		m_pos = {0.0f, 0.0f, 0.0f};
		m_renderer = renderer;
	}

	Object::Object(const glm::vec3& pos, MeshRenderer* renderer)
	{
		m_pos = pos;
		m_renderer = renderer;
	}

	Object::~Object()
	{
		delete m_renderer;
	}

	glm::mat4 Object::getModelMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), m_pos);
	}

	void Object::draw(Shader& shader) const
	{
		m_renderer->draw(shader);
	}
}