#pragma once

#include "FallOffLight.h"

namespace GLEngine
{
	class SpotLight : protected FallOffLight
	{
	private:
		glm::vec3 m_position;
		glm::vec3 m_direction;
		float m_inCutOff;
		float m_outCutOff;

		SpotLight(const glm::vec3& color);

	public:
		SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction, float inCutOff, float outCutOff, float range);

		virtual void SetupShaderProperties(Shader& shader) const override;
	};
}