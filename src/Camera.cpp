#include "Camera.h"

#include <glm/ext.hpp>

namespace GLEngine
{
	Camera::Camera(glm::vec3 pos, float yaw, float pitch, glm::vec3 worldUp, float speed)
	{
		m_pos = pos;

		m_yaw = yaw;
		m_pitch = pitch;
		m_worldUp = worldUp;

		updateVectors();

		m_projMat = glm::transpose(glm::frustum(-0.05, 0.05, -0.05, 0.05, 0.5, 100.0));
	}

	void Camera::updateVectors()
	{
		glm::vec3 forward;
		forward.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		forward.y = sin(glm::radians(m_pitch));
		forward.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_forward = glm::normalize(forward);

		m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_forward));
	}

	void Camera::translate(const glm::vec3 &trans)
	{
		m_pos += trans;
		m_processView = true;
	}

	void Camera::translate(float x, float y, float z)
	{
		translate(glm::vec3(x, y, z));
	}

	void Camera::pitch(float angle)
	{
		m_pitch += angle;
		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		else if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}

		m_processView = true;
	}

	void Camera::yaw(float angle)
	{
		m_yaw += angle;

		m_processView = true;
	}

	void Camera::rotate(float pitchAngle, float yawAngle)
	{
		pitch(pitchAngle);
		yaw(yawAngle);

		updateVectors();
	}

	glm::vec3 Camera::right() const
	{
		return m_right;
	}

	glm::vec3 Camera::up() const
	{
		return m_up;
	}

	glm::vec3 Camera::forward() const
	{
		return m_forward;
	}

	glm::mat4 Camera::getViewMatrix()
	{
		if (m_processView)
		{
			m_viewMat = glm::lookAt(m_pos, m_pos + m_forward, m_worldUp);
			m_processView = false;
		}

		return m_viewMat;
	}

	glm::mat4 Camera::getProjectionMatrix()
	{
		return m_projMat;
	}
}