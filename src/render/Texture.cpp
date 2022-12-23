#include "Texture.h"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <filesystem>

#include "glWrappers/glError.h"

namespace GLEngine
{
	Texture::Texture(const glm::vec2& size)
	{
		m_size = size;

		GL_CALL(glGenTextures(1, &m_id));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_id));

		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}

	Texture::Texture(const glm::vec2& size, unsigned char* data)
	{
		m_size = size;

		GL_CALL(glGenTextures(1, &m_id));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_id));

		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	std::shared_ptr<Texture> Texture::fromFile(const std::string& path)
	{
		auto absPath = std::filesystem::absolute(path).string();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data == nullptr)
		{
			std::cerr << "Failed to load texture at " << path << "\n";
			return 0;
		}

		glm::vec2 size = { width, height };
		auto texture = std::make_shared<Texture>(size, data);

		stbi_image_free(data);

		return texture;
	}

	Texture::~Texture()
	{
		GL_CALL(glDeleteTextures(1, &m_id));
	}

	unsigned int Texture::getId() const
	{
		return m_id;
	}

	void Texture::bind() const
	{
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_id));
	}

	void Texture::unbind() const
	{
		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void Texture::bindToUnit(const int unit) const
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
		bind();
	}
}