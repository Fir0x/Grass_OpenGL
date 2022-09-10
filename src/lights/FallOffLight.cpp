#include "FallOffLight.h"

namespace GLEngine
{
	static float lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}

	static float getLerpCoef(float a, float b, float x)
	{
		return 1 - (b - x) / (b - a);
	}

	FallOffLight::FallOffLight(const glm::vec3& color, float range)
		: Light(color)
	{
		processCoefficients(range);
	}

	void FallOffLight::processCoefficients(float range)
	{
		// Values based OGRE engine (https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation)
		// Lerp is used to allow intermediate radius values between thresholds.
		if (range <= 7)
		{
			m_constant = 1.0f;
			m_linear = 0.7f;
			m_quadratic = 1.8f;
		}
		else if (range <= 13)
		{
			float lerpCoef = getLerpCoef(7, 13, range);
			m_constant = 1.0f;
			m_linear = lerp(0.7f, 0.35f, lerpCoef);
			m_quadratic = lerp(1.8f, 0.44f, lerpCoef);
		}
		else if (range <= 20)
		{
			float lerpCoef = getLerpCoef(13, 20, range);
			m_constant = 1.0f;
			m_linear = lerp(0.35f, 0.22f, lerpCoef);
			m_quadratic = lerp(0.44f, 0.20f, lerpCoef);
		}
		else if (range <= 32)
		{
			float lerpCoef = getLerpCoef(20, 32, range);
			m_constant = 1.0f;
			m_linear = lerp(0.22f, 0.14f, lerpCoef);
			m_quadratic = lerp(0.20f, 0.07f, lerpCoef);
		}
		else if (range <= 50)
		{
			float lerpCoef = getLerpCoef(32, 50, range);
			m_constant = 1.0f;
			m_linear = lerp(0.14f, 0.09f, lerpCoef);
			m_quadratic = lerp(0.07f, 0.032f, lerpCoef);
		}
		else if (range <= 65)
		{
			float lerpCoef = getLerpCoef(50, 65, range);
			m_constant = 1.0f;
			m_linear = lerp(0.09f, 0.07f, lerpCoef);
			m_quadratic = lerp(0.032f, 0.017f, lerpCoef);
		}
		else if (range <= 100)
		{
			float lerpCoef = getLerpCoef(65, 100, range);
			m_constant = 1.0f;
			m_linear = lerp(0.09f, 0.045f, lerpCoef);
			m_quadratic = lerp(0.017f, 0.0075f, lerpCoef);
		}
		else if (range <= 160)
		{
			float lerpCoef = getLerpCoef(100, 160, range);
			m_constant = 1.0f;
			m_linear = lerp(0.045f, 0.027f, lerpCoef);
			m_quadratic = lerp(0.0075f, 0.0028f, lerpCoef);
		}
		else if (range <= 200)
		{
			float lerpCoef = getLerpCoef(160, 200, range);
			m_constant = 1.0f;
			m_linear = lerp(0.027f, 0.022f, lerpCoef);
			m_quadratic = lerp(0.0028f, 0.0019f, lerpCoef);
		}
		else if (range <= 325)
		{
			float lerpCoef = getLerpCoef(200, 325, range);
			m_constant = 1.0f;
			m_linear = lerp(0.022f, 0.014f, lerpCoef);
			m_quadratic = lerp(0.0019f, 0.0007f, lerpCoef);
		}
		else if (range <= 600)
		{
			float lerpCoef = getLerpCoef(325, 600, range);
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
}