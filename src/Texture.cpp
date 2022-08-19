#include "Texture.h"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "glError.h"

namespace GLEngine
{
	Texture::Texture(const std::string& path, const int width, const int height, unsigned char* data)
	{
		m_path = path;
		m_width = width;
		m_height = height;

		GL_CALL(glGenTextures(1, &m_id));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_id));

		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
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

	TextureManager::TextureManager()
	{
		unsigned char defaultData[3] = { 255, 255, 255 };
		auto defaultTex = new Texture("$DEFAULT_TEX", 1, 1, defaultData);
		m_defaultTexId = defaultTex->getId();
		m_textures.insert({ m_defaultTexId, defaultTex });
	}

	unsigned int TextureManager::loadTexture(const std::string& path)
	{
		// TODO
		return 0;
	}

	void TextureManager::deleteTexture(const unsigned int id)
	{
		if (id == m_defaultTexId || m_textures.find(id) == m_textures.end())
			return;

		Texture* texture = m_textures.at(id);
		m_textures.erase(id);
		free(texture);
	}

	const Texture* TextureManager::getTexture(const unsigned int id) const
	{
		if (m_textures.find(id) == m_textures.end())
			return nullptr;

		return m_textures.at(id);
	}

	const Texture* TextureManager::getDefaultTexture() const
	{
		return m_textures.at(m_defaultTexId);
	}
}