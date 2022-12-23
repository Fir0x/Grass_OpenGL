#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

namespace GLEngine
{
	struct ShaderSources
	{
		std::string vertexShader;
		std::string fragmentShader;
	};

	class Program
	{
	private:
		unsigned int m_id;
		std::unordered_map<std::string, int> locations;

		int getUniformLocation(const char* name);

	public:
		Program(const std::string& vertex_src, const std::string& fragment_src);
		~Program();

		static std::shared_ptr<Program> fromFiles(const std::string& vertex_path, const std::string& fragment_path);

		void use();

		void setUniform(const char* name, int v0);
		void setUniform(const char* name, float v0);
		void setUniform(const char* name, float v0, float v1, float v2);
		void setUniform(const char* name, const glm::vec3& v);
		void setUniform(const char* name, float v0, float v1, float v2, float v3);
		void setUniform(const char* name, glm::mat3 matrix, bool transpose = false);
		void setUniform(const char* name, glm::mat4 matrix, bool transpose = false);
	};
}