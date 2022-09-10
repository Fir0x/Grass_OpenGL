#pragma once

#include "Light.h"

namespace GLEngine
{
	class SpotLight : Light
	{
	private:
		glm::vec3 m_position;
		glm::vec3 m_direction;
		float m_cutoff;

		SpotLight(const glm::vec3& color);

		friend class Light;

	public:
		SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction, float cutoff);

		virtual void SetupShaderProperties(Shader& shader) const override;
	};
}