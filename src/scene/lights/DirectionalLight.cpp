#include "DirectionalLight.h"

namespace GLEngine
{
	DirectionalLight::DirectionalLight(const glm::vec3& color)
		: Light(color)
	{
		m_direction = glm::vec3(0.0f);
	}

	DirectionalLight::DirectionalLight(const glm::vec3& color, const glm::vec3& direction)
		: Light(color)
	{
		m_direction = glm::normalize(direction);
	}

	void DirectionalLight::SetupShaderProperties(Program& shader, int i) const
	{
		std::string prefix = "dirLight[" + std::to_string(i) + "].";
		shader.setUniform((prefix + "color").c_str(), m_color);
		shader.setUniform((prefix + "direction").c_str(), m_direction);
	}
}
