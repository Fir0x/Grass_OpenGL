#pragma once

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

	class Shader
	{
	private:
		unsigned int m_id;
		std::unordered_map<const char*, int> locations;

		ShaderSources parse_shader_file(const char* path);
		unsigned int compile_shader(const std::string &path, unsigned int type);
		int getUniformLocation(const char* name);

	public:
		Shader(const char* path);
		~Shader();

		void use();

		void setUniform3f(const char* name, float v0, float v1, float v2);
		void setUniform4f(const char* name, float v0, float v1, float v2, float v3);
		void setUniformMatrix4f(const char* name, glm::mat4 matrix, bool transpose = false);
	};
}