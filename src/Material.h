#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>

#include "Shader.h"

namespace GLEngine
{
	class Material
	{
	private:
		std::string m_name;
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;

		unsigned int m_diffuseTex;
		unsigned int m_specularTex;

	public:
		Material(const std::string& name);

		Material& setName(const std::string& name);
		Material& setAmbient(const glm::vec3& ambient);
		Material& setDiffuse(const glm::vec3& diffuse);
		Material& setSpecular(const glm::vec3& specular);
		Material& setDiffuseTex(const std::string& path);
		Material& setSpecularTex(const std::string& path);

		void loadToGPU(Shader& shader) const;

		static std::optional<Material> loadFromMtl(const std::string path);
	};
}