#pragma once

#include <glm/glm.hpp>
#include <map>
#include <optional>
#include <string>

#include "Program.h"
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

		std::shared_ptr<Texture> m_diffuseTex;
		std::shared_ptr<Texture> m_specularTex;

	public:
		Material(const std::string& name);

		Material& setName(const std::string& name);
		Material& setAmbient(const glm::vec3& ambient);
		Material& setDiffuse(const glm::vec3& diffuse);
		Material& setSpecular(const glm::vec3& specular);
		Material& setDiffuseTex(const std::string& path);
		Material& setSpecularTex(const std::string& path);

		void loadToGPU(Program& shader) const;

		static std::optional<Material> loadFromMtl(const std::string path);
	};

	class MaterialLibrary
	{
	private:
		std::map<unsigned int, Material> m_library;
		unsigned int m_currentId;
		MaterialLibrary();

		static MaterialLibrary& getInstance();

	public:
		static unsigned int loadFromMtl(const std::string& path);
		static void useMaterial(unsigned int id, Program& shader);
		static unsigned int getDefaultId();
	};
}