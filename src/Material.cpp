#include "Material.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace GLEngine
{
	Material::Material(const std::string& name)
	{
		m_name = name;
		m_ambient = glm::vec3(1.0f);
		m_diffuse = glm::vec3(1.0f);
		m_specular = glm::vec3(1.0f);

		// TODO
		// Use a default texture
		m_diffuseTex = 0;
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

	Material& Material::setDiffuseTex(const std::string& path, TextureManager& texManager)
	{
		m_diffuseTex = texManager.loadTexture(path);
		return *this;
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
			}

			return material;
		}
		else
			return std::optional<Material>();
	}
}