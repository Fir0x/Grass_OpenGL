#include "Material.h"

namespace GLEngine
{
	MaterialFactory::MaterialFactory()
	{
		reset();
	}

	MaterialFactory& MaterialFactory::setName(const std::string& name)
	{
		m_name = name;
		return *this;
	}

	MaterialFactory& MaterialFactory::setAmbient(const glm::vec3& ambient)
	{
		m_ambient = ambient;
		return *this;
	}

	MaterialFactory& MaterialFactory::setDiffuse(const glm::vec3& diffuse)
	{
		m_diffuse = diffuse;
		return *this;
	}

	MaterialFactory& MaterialFactory::setSpecular(const glm::vec3& specular)
	{
		m_specular = specular;
		return *this;
	}

	MaterialFactory& MaterialFactory::setDiffuseTex(const std::string& path)
	{
		m_diffuseTexPath = path;
		return *this;
	}

	Material MaterialFactory::generateMaterial(TextureManager& texManager) const
	{
		Material mat;
		mat.name = m_name;
		mat.ambient = m_ambient;
		mat.diffuse = m_diffuse;
		mat.specular = m_specular;
		mat.diffuseTex = m_diffuseTexPath.empty() ? 0 : texManager.loadTexture(m_diffuseTexPath);

		return mat;
	}

	void MaterialFactory::reset()
	{
		m_name = "";
		m_ambient = glm::vec3(1.0f);
		m_diffuse = glm::vec3(1.0f);
		m_specular = glm::vec3(1.0f);
		m_diffuseTexPath = "";
	}
}