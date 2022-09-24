#pragma once

#include <glm/glm.hpp>

namespace GLEngine
{
	class Transform
	{
	private:
		glm::mat4 m_transforms;

	public:
		Transform();
		Transform(const glm::vec3& position);

		void translate(const glm::vec3& translation);
		/// <summary>
		/// Rotate around Z-axis, then Y-axis, and finally X-axis.
		/// </summary>
		/// <param name="angleX"></param>
		/// <param name="angleY"></param>
		/// <param name="angleZ"></param>
		void rotate(float angleX, float angleY, float angleZ);
		const glm::mat4& getMatrix() const;
	};
}