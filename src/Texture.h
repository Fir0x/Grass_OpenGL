#pragma once

#include <string>
#include <map>
#include <vector>

namespace GLEngine
{
	class Texture
	{
	private:
		unsigned int m_id;
		std::string m_path;
		int m_width;
		int m_height;

	public:
		Texture(const std::string& path, const int width, const int height, unsigned char* data);
		~Texture();

		unsigned int getId() const;
		const std::string& getPath() const;

		void bind() const;
		void unbind() const;

		void bindToUnit(const int unit) const;
	};

	class TextureManager
	{
	private:
		std::map<std::string, unsigned int> m_pathMapper;
		std::map<unsigned int, Texture*> m_textures;
		unsigned int m_defaultTexId;

	public:
		TextureManager();
		~TextureManager();

		unsigned int loadTexture(const std::string& path);
		void deleteTexture(const unsigned int id);
		const Texture* getTexture(const unsigned int id) const;
		const Texture* getDefaultTexture() const;
	};
}