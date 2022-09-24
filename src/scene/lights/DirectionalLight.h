#pragma once

#include "Light.h"

namespace GLEngine
{
	class DirectionalLight : Light
	{
	private:
		glm::vec3 m_direction;

		DirectionalLight(const glm::vec3& color);

		friend class Light;

	public:
		DirectionalLight(const glm::vec3& color, const glm::vec3& direction);

		virtual void SetupShaderProperties(Shader& shader, int i) const override;
	};
}