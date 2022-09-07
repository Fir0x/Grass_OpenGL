#pragma once

#include <glm/glm.hpp>

#include "../Shader.h"

namespace GLEngine
{
	class Light
	{
	protected:
		glm::vec3 m_color;

	public:
		Light(const glm::vec3& color);

		/// <summary>
		/// Set shader uniforms needed to compute the light contribution.
		/// </summary>
		/// <param name="shader"></param>
		virtual void SetupShaderProperties(Shader& shader) const = 0;

		template <class T>
		static T getFiller()
		{
			static_assert(std::is_base_of_v<Light, T>);

			glm::vec3 black(0.0f, 0.0f, 0.0f);
			return T(black);
		}
	};
}