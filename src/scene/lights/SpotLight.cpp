#include "SpotLight.h"

#include <iostream>

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

	void SpotLight::SetupShaderProperties(Shader& shader, int i) const
	{
		std::string prefix = "spotLight[" + std::to_string(i) + "].";
		shader.setUniform3f((prefix + "color").c_str(), m_color);
		shader.setUniform3f((prefix + "position").c_str(), m_position);
		shader.setUniform3f((prefix + "direction").c_str(), m_direction);
		shader.setUniform1f((prefix + "inCutOff").c_str(), glm::cos(glm::radians(m_inCutOff)));
		shader.setUniform1f((prefix + "outCutOff").c_str(), glm::cos(glm::radians(m_outCutOff)));

		shader.setUniform1f((prefix + "constant").c_str(), m_constant);
		shader.setUniform1f((prefix + "linear").c_str(), m_linear);
		shader.setUniform1f((prefix + "quadratic").c_str(), m_quadratic);
	}
}