#include "Camera.h"

#include <glm/ext.hpp>
#include <iostream>

namespace GLEngine
{
	Camera::Camera(glm::mat4 frustum, glm::vec3 pos, float yaw, float pitch, glm::vec3 worldUp, float speed)
	{
		initialize(frustum, pos, yaw, pitch, worldUp, speed);
	}

	Camera::Camera(glm::vec3 pos, float yaw, float pitch, glm::vec3 worldUp, float speed)
	{
		initialize(glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f), pos, yaw, pitch, worldUp, speed);
	}


	void Camera::initialize(glm::mat4 frustum, glm::vec3 pos, float yaw, float pitch, glm::vec3 worldUp, float speed)
	{
		m_pos = pos;

		m_yaw = yaw;
		m_pitch = pitch;
		m_worldUp = worldUp;

		updateVectors();

		m_projMat = frustum;
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

	glm::mat4 Camera::getProjectionMatrix() const
	{
		return m_projMat;
	}

	glm::vec3 Camera::getPosition() const
	{
		return m_pos;
	}

	void Camera::onScreenSizeChange(int w, int h)
	{
		m_projMat = glm::perspective(45.0f, (float)w / (float)h, 0.1f, 100.0f);
	}
}