#pragma once

#include "FallOffLight.h"

namespace GLEngine
{
	class PointLight : protected FallOffLight
	{
	private:
		glm::vec3 m_position;
		float m_radius;

		PointLight(const glm::vec3& color);

	public:
		PointLight(const glm::vec3& color, const glm::vec3& position, float range);

		virtual void SetupShaderProperties(Shader& shader, int i) const override;
	};
}