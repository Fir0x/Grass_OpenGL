#include "Transform.h"

#include <glm/ext.hpp>

namespace GLEngine
{
	Transform::Transform()
	{
		m_transforms = glm::mat4(1.0f);
	}

	Transform::Transform(const glm::vec3& position)
	{
		m_transforms = glm::translate(glm::mat4(1.0f), position);
	}

	void Transform::translate(const glm::vec3& translation)
	{
		m_transforms = glm::translate(m_transforms, translation);
	}

	void Transform::rotate(float angleX, float angleY, float angleZ)
	{
		if (angleZ != 0)
		{
			glm::vec3 zAxis = glm::column(m_transforms, 2);
			m_transforms = glm::rotate(m_transforms, angleZ, zAxis);
		}

		if (angleY != 0)
		{
			glm::vec3 yAxis = glm::column(m_transforms, 1);
			m_transforms = glm::rotate(m_transforms, angleY, yAxis);
		}

		if (angleX != 0)
		{
			glm::vec3 xAxis = glm::column(m_transforms, 0);
			m_transforms = glm::rotate(m_transforms, angleX, xAxis);
		}
	}

	const glm::mat4& Transform::getMatrix() const
	{
		return m_transforms;
	}
}