#pragma once

#include "Light.h"

namespace GLEngine
{
	class FallOffLight : protected Light
	{
	protected:
		float m_constant;
		float m_linear;
		float m_quadratic;

		FallOffLight(const glm::vec3& color, float range);

		/// <summary>
		/// Process the attenuation coefficient from the current radius of the light.
		/// </summary>
		/// <param name="range">Range of the light.</param>
		void processCoefficients(float range);

		friend class Light;
	};
}