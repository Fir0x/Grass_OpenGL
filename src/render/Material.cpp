#include "Material.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Texture.h"

namespace GLEngine
{

	Material::Material(const std::string& name)
	{
		m_name = name;
		m_ambient = glm::vec3(1.0f);
		m_diffuse = glm::vec3(1.0f);
		m_specular = glm::vec3(1.0f);

		m_diffuseTex = nullptr;
		m_specularTex = nullptr;
	}

	Material& Material::setName(const std::string& name)
	{
		m_name = name;
		return *this;
	}

	Material& Material::setAmbient(const glm::vec3& ambient)
	{
		m_ambient = ambient;
		return *this;
	}

	Material& Material::setDiffuse(const glm::vec3& diffuse)
	{
		m_diffuse = diffuse;
		return *this;
	}

	Material& Material::setSpecular(const glm::vec3& specular)
	{
		m_specular = specular;
		return *this;
	}

	Material& Material::setDiffuseTex(const std::string& path)
	{
		m_diffuseTex = Texture::fromFile(path);
		return *this;
	}

	Material& Material::setSpecularTex(const std::string& path)
	{
		m_specularTex = Texture::fromFile(path);
		return *this;
	}

	void Material::loadToGPU(Program& shader) const
	{
		shader.setUniform("material.diffuse", m_diffuse);
		shader.setUniform("material.specular", m_ambient);
		shader.setUniform("material.shininess", 32.0f);
		if (m_diffuseTex)
		{
			m_diffuseTex->bindToUnit(0);
			shader.setUniform("material.diffuseTex", 0);
			shader.setUniform("useDiffuseTex", 1);
		}
		else
			shader.setUniform("useDiffuseTex", 0);

		if (m_specularTex)
		{
			m_specularTex->bindToUnit(1);
			shader.setUniform("material.specularTex", 1);
			shader.setUniform("useSpecularTex", 1);
		}
		else
			shader.setUniform("useSpecularTex", 0);
	}

	static std::vector<std::string> splitstr(const std::string& str, char delim)
	{
		std::vector<std::string> splitted;

		std::istringstream stream(str);
		std::string word;
		while (std::getline(stream, word, delim))
		{
			splitted.push_back(word);
		}

		return splitted;
	}

	std::optional<Material> Material::loadFromMtl(const std::string path)
	{
		std::ifstream stream;
		stream.open(path);

		if (stream.is_open())
		{
			Material material("placeholder");
			bool secondMtl = false;
			std::string line;

			while (std::getline(stream, line))
			{
				if (line[0] == '#' || line.empty())
					continue;

				auto splitted = splitstr(line, ' ');
				const std::string& tag = splitted[0];
				if (tag == "newmtl")
				{
					if (secondMtl)
					{
						std::cerr << "WARNING: " << path
							<< " contained more than one material definition. "
							<< "Only the first one was parsed.";
						return material;
					}

					material.setName(splitted[1]);
				}
				else if (tag == "Ka")
				{
					glm::vec3 ambient;
					ambient.r = std::stof(splitted[1]);
					ambient.g = std::stof(splitted[2]);
					ambient.b = std::stof(splitted[3]);
					material.setAmbient(ambient);
				}
				else if (tag == "Kd")
				{
					glm::vec3 diffuse;
					diffuse.r = std::stof(splitted[1]);
					diffuse.g = std::stof(splitted[2]);
					diffuse.b = std::stof(splitted[3]);
					material.setDiffuse(diffuse);
				}
				else if (tag == "Ks")
				{
					glm::vec3 specular;
					specular.r = std::stof(splitted[1]);
					specular.g = std::stof(splitted[2]);
					specular.b = std::stof(splitted[3]);
					material.setSpecular(specular);
				}
				else if (tag == "map_Kd")
				{
					material.setDiffuseTex(splitted[1]);
				}
				else if (tag == "map_Ks")
				{
					material.setSpecularTex(splitted[1]);
				}
			}

			return material;
		}
		else
			return std::optional<Material>();
	}

	MaterialLibrary::MaterialLibrary()
	{
		m_library.insert({ 1, Material("$DEFAULT") });
		m_currentId = 2;
	}

	MaterialLibrary& MaterialLibrary::getInstance()
	{
		static MaterialLibrary instance;
		return instance;
	}

	unsigned int MaterialLibrary::loadFromMtl(const std::string& path)
	{
		auto material = Material::loadFromMtl(path);
		if (material.has_value())
		{
			MaterialLibrary& instance = getInstance();
			unsigned int id = instance.m_currentId++;
			instance.m_library.insert({ id, material.value() });
			return id;
		}
		
		return 0;
	}

	void MaterialLibrary::useMaterial(unsigned int id, Program& shader)
	{
		MaterialLibrary& instance = getInstance();
		if (instance.m_library.find(id) != instance.m_library.end())
		{
			instance.m_library.at(id).loadToGPU(shader);
		}
	}

	unsigned int MaterialLibrary::getDefaultId()
	{
		return 1;
	}
}