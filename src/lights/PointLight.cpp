#include "PointLight.h"

namespace GLEngine
{
	PointLight::PointLight(const glm::vec3& color)
		: Light(color)
	{
		m_position = glm::vec3(0.0f);

		m_radius = 0;

		processCoefficients();
	}

	PointLight::PointLight(const glm::vec3& color, const glm::vec3& position, const int radius)
		: Light(color)
	{
		m_position = position;
		m_radius = radius;

		processCoefficients();
	}

	static float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	static float getLerpCoef(int a, int b, int x)
	{
		return 1 - float(b - x) / float(b - a);
	}

	void PointLight::processCoefficients()
	{
		// Values based OGRE engine (https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation)
		// Lerp is used to allow intermediate radius values between thresholds.
		if (m_radius <= 7)
		{
			m_constant = 1.0f;
			m_linear = 0.7f;
			m_quadratic = 1.8f;
		}
		else if (m_radius <= 13)
		{
			float lerpCoef = getLerpCoef(7, 13, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.7f, 0.35f, lerpCoef);
			m_quadratic = lerp(1.8f, 0.44f, lerpCoef);
		}
		else if (m_radius <= 20)
		{
			float lerpCoef = getLerpCoef(13, 20, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.35f, 0.22f, lerpCoef);
			m_quadratic = lerp(0.44f, 0.20f, lerpCoef);
		}
		else if (m_radius <= 32)
		{
			float lerpCoef = getLerpCoef(20, 32, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.22f, 0.14f, lerpCoef);
			m_quadratic = lerp(0.20f, 0.07f, lerpCoef);
		}
		else if (m_radius <= 50)
		{
			float lerpCoef = getLerpCoef(32, 50, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.14f, 0.09f, lerpCoef);
			m_quadratic = lerp(0.07f, 0.032f, lerpCoef);
		}
		else if (m_radius <= 65)
		{
			float lerpCoef = getLerpCoef(50, 65, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.09f, 0.07f, lerpCoef);
			m_quadratic = lerp(0.032f, 0.017f, lerpCoef);
		}
		else if (m_radius <= 100)
		{
			float lerpCoef = getLerpCoef(65, 100, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.09f, 0.045f, lerpCoef);
			m_quadratic = lerp(0.017f, 0.0075f, lerpCoef);
		}
		else if (m_radius <= 160)
		{
			float lerpCoef = getLerpCoef(100, 160, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.045f, 0.027f, lerpCoef);
			m_quadratic = lerp(0.0075f, 0.0028f, lerpCoef);
		}
		else if (m_radius <= 200)
		{
			float lerpCoef = getLerpCoef(160, 200, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.027f, 0.022f, lerpCoef);
			m_quadratic = lerp(0.0028f, 0.0019f, lerpCoef);
		}
		else if (m_radius <= 325)
		{
			float lerpCoef = getLerpCoef(200, 325, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.022f, 0.014f, lerpCoef);
			m_quadratic = lerp(0.0019f, 0.0007f, lerpCoef);
		}
		else if (m_radius <= 600)
		{
			float lerpCoef = getLerpCoef(325, 600, m_radius);
			m_constant = 1.0f;
			m_linear = lerp(0.014f, 0.007f, lerpCoef);
			m_quadratic = lerp(0.0007f, 0.0002f, lerpCoef);
		}
		else
		{
			m_constant = 1.0f;
			m_linear = 0.0014f;
			m_quadratic = 0.000007f;
		}
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