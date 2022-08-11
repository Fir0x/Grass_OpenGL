#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace GLEngine
{
	class Camera
	{
	private:
		glm::vec3 m_pos;
		glm::vec3 m_right;
		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_worldUp;

		float m_pitch = 0;
		float m_yaw = 0;

		glm::mat4 m_projMat;
		bool m_processView = true;
		glm::mat4 m_viewMat;

		void pitch(float angle);
		void yaw(float angle);

		void initialize(glm::mat4 frustum, glm::vec3 pos, float yaw,
			float pitch, glm::vec3 worldUp, float speed);

		void updateVectors();

	public:
		Camera(glm::mat4 frustum = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f),
			glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
			float yaw = -90.0f,
			float pitch = 0.0f,
			glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
			float speed = 2.5f);

		Camera(glm::vec3 pos,
			float yaw = -90.0f,
			float pitch = 0.0f,
			glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
			float speed = 2.5f);

		void translate(const glm::vec3& trans);
		void translate(float x, float y, float z);

		void rotate(float pitchAngle, float yawAngle);

		glm::vec3 right() const;
		glm::vec3 up() const;
		glm::vec3 forward() const;

		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
	};
}

