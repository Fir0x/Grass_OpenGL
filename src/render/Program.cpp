#include "Program.h"

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "glWrappers/glError.h"

namespace GLEngine
{
	static unsigned int compileShader(const std::string& source, unsigned int type)
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

	static bool linkProgram(unsigned int programId)
	{
		GL_CALL(glLinkProgram(programId));

		GLint link_status = GL_TRUE;
		GL_CALL(glGetProgramiv(programId, GL_LINK_STATUS, &link_status));

		if (link_status != GL_TRUE) {
			GLint log_size;
			char* log;
			GL_CALL(glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &log_size));

			log = (char*)std::malloc((size_t)log_size + 1);
			if (log != nullptr) {
				GL_CALL(glGetProgramInfoLog(programId, log_size, &log_size, log));
				std::cerr << "Failed to link program.\n" << log << std::endl;
				std::free(log);
			}

			return false;
		}

		return true;
	}

	Program::Program(const std::string& vertex_src, const std::string& fragment_src)
	{
		m_id = glCreateProgram(); GL_AFTER_CHECK();

		auto vertex = compileShader(vertex_src, GL_VERTEX_SHADER);
		auto fragment = compileShader(fragment_src, GL_FRAGMENT_SHADER);

		if (vertex == 0)
			std::cerr << "Compilation failed for vertex shader." << std::endl;

		if (fragment == 0)
			std::cerr << "Compilation failed for fragment shader." << std::endl;

		GL_CALL(glAttachShader(m_id, vertex));
		GL_CALL(glAttachShader(m_id, fragment));

		bool linkSuccess = linkProgram(m_id);

		GL_CALL(glDeleteShader(vertex));
		GL_CALL(glDeleteShader(fragment));

		if (!linkSuccess)
		{
			GL_CALL(glDeleteProgram(m_id));
		}
	}

	Program::~Program()
	{
		GL_CALL(glDeleteProgram(m_id));
	}

	static std::string loadShaderFile(const std::string& path)
	{
		std::ifstream stream(path);
		std::stringstream shader;
		if (stream.is_open())
			shader << stream.rdbuf();

		return shader.str();
	}

	std::shared_ptr<Program> Program::fromFiles(const std::string& vertex_path, const std::string& fragment_path)
	{
		auto vertex_src = loadShaderFile(vertex_path);
		auto fragment_src = loadShaderFile(fragment_path);

		return std::make_shared<Program>(vertex_src, fragment_src);
	}

	void Program::use()
	{
		GL_CALL(glUseProgram(m_id));
	}

	int Program::getUniformLocation(const char* name)
	{
		if (locations.find(name) == locations.end())
		{
			int l = glGetUniformLocation(m_id, name); GL_AFTER_CHECK();
			locations.insert({ name, l });
		}

		return locations.at(name);
	}

	void Program::setUniform(const char* name, int v0)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniform1i(location, v0));
	}

	void Program::setUniform(const char* name, float v0)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniform1f(location, v0));
	}

	void Program::setUniform(const char* name, float v0, float v1, float v2)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniform3f(location, v0, v1, v2));
	}

	void Program::setUniform(const char* name, const glm::vec3& v)
	{
		setUniform(name, v.x, v.y, v.z);
	}

	void Program::setUniform(const char* name, float v0, float v1, float v2, float v3)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniform4f(location, v0, v1, v2, v3));
	}

	void Program::setUniform(const char* name, glm::mat3 matrix, bool transpose)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniformMatrix3fv(location, 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix)));
	}

	void Program::setUniform(const char* name, glm::mat4 matrix, bool transpose)
	{
		int location = getUniformLocation(name);
		GL_CALL(glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix)));
	}
}