#pragma once

#include <glm/glm.hpp>
#include <string>

#include "Texture.h"

namespace GLEngine
{
	struct Material
	{
		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		unsigned int diffuseTex;
	};

	class MaterialFactory
	{
	private:
		std::string m_name;
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;

		std::string m_diffuseTexPath;

	public:
		MaterialFactory();

		MaterialFactory& setName(const std::string& name);
		MaterialFactory& setAmbient(const glm::vec3& ambient);
		MaterialFactory& setDiffuse(const glm::vec3& diffuse);
		MaterialFactory& setSpecular(const glm::vec3& specular);
		MaterialFactory& setDiffuseTex(const std::string& path);

		Material generateMaterial(TextureManager& texManager) const;
		void reset();
	};
}