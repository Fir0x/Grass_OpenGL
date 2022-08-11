#include "Shader.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "glError.h"

namespace GLEngine
{
	ShaderSources Shader::parse_shader_file(const char *path)
	{
		ShaderSources sources;

		std::ifstream stream(path);
		if (stream.is_open())
		{
			enum class ShaderType
			{
				NONE = -1,
				VERTEX = 0,
				FRAGMENT = 1
			};

			std::string line;
			ShaderType type = ShaderType::NONE;
			std::stringstream shaders[2];
			while (getline(stream, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = ShaderType::VERTEX;
					else if (line.find("fragment") != std::string::npos)
						type = ShaderType::FRAGMENT;
				}
				else if (type != ShaderType::NONE)
					shaders[int(type)] << line << "\n";
			}

			sources = { shaders[0].str(), shaders[1].str() };
		}

		return sources;
	}

	unsigned int Shader::compile_shader(const std::string& source, unsigned int type)
	{
		unsigned int id = glCreateShader(type); GL_AFTER_CHECK();

		const GLchar* src = source.c_str();
		GL_CALL(glShaderSource(id, 1, &src, 0));
		GL_CALL(glCompileShader(id));

		GLint compile_status = GL_TRUE;
		GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status));

		if (compile_status != GL_TRUE) {
			GLint log_size;
			GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_size));
			char* log = (char*)std::malloc(log_size * sizeof(char));
			if (log != nullptr) {
				GL_CALL(glGetShaderInfoLog(id, log_size, &log_size, log));
				std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
					<< " shader.\n";
				std::cerr << log << std::endl;
				std::free(log);
			}

			GL_CALL(glDeleteShader(id));

			return 0;
		}

		return id;

	}

	int Shader::getUniformLocation(const char* name)
	{
		if (locations.find(name) == locations.end())
		{
			locations[name] = glGetUniformLocation(m_id, name); GL_AFTER_CHECK();
		}

		return locations[name];
	}

	Shader::Shader(const char* path)
	{
		m_id = glCreateProgram(); GL_AFTER_CHECK();

		auto sources = parse_shader_file(path);
		auto vertex = compile_shader(sources.vertexShader, GL_VERTEX_SHADER);
		auto fragment = compile_shader(sources.fragmentShader, GL_FRAGMENT_SHADER);

		if (vertex == 0 || fragment == 0)
			std::cerr << "Errors in shader file: " << path << std::endl;

		GL_CALL(glAttachShader(m_id, vertex));
		GL_CALL(glAttachShader(m_id, fragment));

		GL_CALL(glLinkProgram(m_id));

		GLint link_status = GL_TRUE;
		GL_CALL(glGetProgramiv(m_id, GL_LINK_STATUS, &link_status));

		if (link_status != GL_TRUE) {
			GLint log_size;
			char* log;
			GL_CALL(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &log_size));

			log = (char*)std::malloc(log_size + 1); /* +1 pour le caractere de fin de chaine '\0' */
			if (log != nullptr) {
				GL_CALL(glGetProgramInfoLog(m_id, log_size, &log_size, log));
				std::cerr << "Failed to link program.\n" << log << std::endl;
				std::free(log);
			}

			GL_CALL(glDetachShader(m_id, vertex));
			GL_CALL(glDetachShader(m_id, fragment));

			GL_CALL(glDeleteProgram(m_id));
		}

		GL_CALL(glDeleteShader(vertex));
		GL_CALL(glDeleteShader(fragment));
	}

	Shader::~Shader()
	{
		GL_CALL(glDeleteProgram(m_id));
	}

	void Shader::use()
	{
		GL_CALL(glUseProgram(m_id));
	}

	void Shader::setUniform1i(const char* name, int v0)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniform1i(location, v0));
	}

	void Shader::setUniform3f(const char* name, float v0, float v1, float v2)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniform3f(location, v0, v1, v2));
	}

	void Shader::setUniform4f(const char* name, float v0, float v1, float v2, float v3)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniform4f(location, v0, v1, v2, v3));
	}

	void Shader::setUniformMatrix4f(const char* name, glm::mat4 matrix, bool transpose)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix)));
	}
}