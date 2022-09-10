#include "PointLight.h"

namespace GLEngine
{
	PointLight::PointLight(const glm::vec3& color)
		: FallOffLight(color, 0)
	{
		m_position = glm::vec3(0.0f);

		m_radius = 0;

		processCoefficients(m_radius);
	}

	PointLight::PointLight(const glm::vec3& color, const glm::vec3& position, float range)
		: FallOffLight(color, range)
	{
		m_position = position;
		m_radius = range;
	}

	void PointLight::SetupShaderProperties(Shader& shader) const
	{
		shader.setUniform3f("pointLight.color", m_color);
		shader.setUniform3f("pointLight.position", m_position);

		shader.setUniform1f("pointLight.constant", m_constant);
		shader.setUniform1f("pointLight.linear", m_linear);
		shader.setUniform1f("pointLight.quadratic", m_quadratic);
	}
}