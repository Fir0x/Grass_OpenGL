#include "Texture.h"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <filesystem>

#include "glWrappers/glError.h"

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

	const std::string& Texture::getPath() const
	{
		return m_path;
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

	TextureManager& TextureManager::getInstance()
	{
		static TextureManager texManager;
		return texManager;
	}

	unsigned int TextureManager::loadTexture(const std::string& path)
	{
		auto absPath = std::filesystem::absolute(path).string();
		TextureManager& instance = getInstance();
		if (instance.m_pathMapper.find(absPath) != instance.m_pathMapper.end())
			return instance.m_textures[instance.m_pathMapper[absPath]]->getId();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data == nullptr)
		{
			std::cerr << "Failed to load texture at " << path << "\n";
			return 0;
		}

		auto texture = new Texture(absPath, width, height, data);
		auto texId = texture->getId();
		instance.m_pathMapper.insert({ absPath, texId });
		instance.m_textures.insert({ texId, texture });

		stbi_image_free(data);

		return texId;
	}

	void TextureManager::deleteTexture(const unsigned int id)
	{
		TextureManager& instance = getInstance();
		if (id == instance.m_defaultTexId || instance.m_textures.find(id) == instance.m_textures.end())
			return;

		Texture* texture = instance.m_textures.at(id);
		auto path = texture->getPath();
		if (path != "$DEFAULT_TEX")
		{
			instance.m_pathMapper.erase(path);
			instance.m_textures.erase(id);
			free(texture);
		}
	}

	const Texture* TextureManager::getTexture(const unsigned int id)
	{
		TextureManager& instance = getInstance();
		if (instance.m_textures.find(id) == instance.m_textures.end())
			return nullptr;

		return instance.m_textures.at(id);
	}

	const Texture* TextureManager::getDefaultTexture()
	{
		TextureManager& instance = getInstance();
		return instance.m_textures.at(instance.m_defaultTexId);
	}

	void TextureManager::clean()
	{
		TextureManager& instance = getInstance();
		for (auto iter = instance.m_textures.begin(); iter != instance.m_textures.end(); iter++)
			delete iter->second;
	}
}