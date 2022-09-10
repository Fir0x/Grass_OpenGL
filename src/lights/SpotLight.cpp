#include "SpotLight.h"

namespace GLEngine
{
	SpotLight::SpotLight(const glm::vec3& color)
		: Light(color)
	{
		m_position = glm::vec3(0.0f);
		m_direction = glm::vec3(0.0f);
		m_cutoff = 0;
	}

	SpotLight::SpotLight(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction, float cutoff)
		: Light(color)
	{
		m_position = position;
		m_direction = direction;
		m_cutoff = cutoff;
	}

	void SpotLight::SetupShaderProperties(Shader& shader) const
	{
		shader.setUniform3f("spotLight.color", m_color);
		shader.setUniform3f("spotLight.position", m_position);
		shader.setUniform3f("spotLight.direction", m_direction);
		shader.setUniform1f("spotLight.cutoff", glm::cos(glm::radians(m_cutoff)));
	}
}