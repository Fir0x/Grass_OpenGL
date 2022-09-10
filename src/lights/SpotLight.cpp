#include "SpotLight.h"

namespace GLEngine
{
	SpotLight::SpotLight(const glm::vec3& color)
		: FallOffLight(color, 0)
	{
		m_position = glm::vec3(0.0f);
		m_direction = glm::vec3(0.0f);
		m_inCutOff = 0;
		m_outCutOff = 0;
	}

	SpotLight::SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction, float inCutOff, float outCutOff, float range)
		: FallOffLight(color, range)
	{
		m_position = position;
		m_direction = direction;
		m_inCutOff = inCutOff;
		m_outCutOff = outCutOff;
	}

	void SpotLight::SetupShaderProperties(Shader& shader) const
	{
		shader.setUniform3f("spotLight.color", m_color);
		shader.setUniform3f("spotLight.position", m_position);
		shader.setUniform3f("spotLight.direction", m_direction);
		shader.setUniform1f("spotLight.inCutOff", glm::cos(glm::radians(m_inCutOff)));
		shader.setUniform1f("spotLight.outCutOff", glm::cos(glm::radians(m_outCutOff)));

		shader.setUniform1f("spotLight.constant", m_constant);
		shader.setUniform1f("spotLight.linear", m_linear);
		shader.setUniform1f("spotLight.quadratic", m_quadratic);
	}
}