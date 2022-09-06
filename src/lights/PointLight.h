#pragma once

#include "Light.h"

namespace GLEngine
{
	class PointLight : Light
	{
	private:
		glm::vec3 m_position;
		int m_radius;

		float m_constant;
		float m_linear;
		float m_quadratic;

		PointLight(const glm::vec3& color);

		/// <summary>
		/// Process the attenuation coefficient from the current radius of the light.
		/// </summary>
		void processCoefficients();

		friend class Light;

	public:
		PointLight(const glm::vec3& color, const glm::vec3& position, const int radius);

		virtual void SetupShaderProperties(Shader& shader) const override;
	};
}