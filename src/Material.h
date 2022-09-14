#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>

#include "Texture.h"

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

	public:
		Material(const std::string& name);

		Material& setName(const std::string& name);
		Material& setAmbient(const glm::vec3& ambient);
		Material& setDiffuse(const glm::vec3& diffuse);
		Material& setSpecular(const glm::vec3& specular);
		Material& setDiffuseTex(const std::string& path, TextureManager& texManager);

		static std::optional<Material> loadFromMtl(const std::string path);
	};
}