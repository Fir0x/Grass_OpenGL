#include "Object.h"

#include <GL/glew.h>
#include <glm/ext.hpp>

#include "glWrappers/glError.h"

namespace GLEngine
{
	Object::Object(MeshRenderer* renderer)
	{
		m_transform = Transform();
		m_renderer = renderer;
	}

	Object::Object(const glm::vec3& pos, MeshRenderer* renderer)
	{
		m_transform = Transform(pos);
		m_renderer = renderer;
	}

	Object::~Object()
	{
		delete m_renderer;
	}

	glm::mat3 Object::processNormalMatrix(const glm::mat4& model, const glm::mat4& view) const
	{
		return glm::mat3(glm::transpose(glm::inverse(view * model)));
	}

	Transform& Object::getTransform()
	{
		return m_transform;
	}

	void Object::draw(Shader& shader, Camera& camera) const
	{
		const glm::mat4& modelMatrix = m_transform.getMatrix();
		shader.setUniformMatrix4f("modelMatrix", modelMatrix);
		shader.setUniformMatrix3f("normalMatrix", processNormalMatrix(modelMatrix, camera.getViewMatrix()));
		m_renderer->draw(shader);
	}
}