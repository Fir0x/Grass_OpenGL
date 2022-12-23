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

	void PointLight::SetupShaderProperties(Program& shader, int i) const
	{
		std::string prefix = "pointLight[" + std::to_string(i) + "].";
		shader.setUniform((prefix + "color").c_str(), m_color);
		shader.setUniform((prefix + "position").c_str(), m_position);

		shader.setUniform((prefix + "constant").c_str(), m_constant);
		shader.setUniform((prefix + "linear").c_str(), m_linear);
		shader.setUniform((prefix + "quadratic").c_str(), m_quadratic);
	}
}