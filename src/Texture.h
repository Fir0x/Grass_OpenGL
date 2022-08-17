#pragma once

#include <string>
#include <set>
#include <map>

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

		void bind() const;
		void unbind() const;

		void bindToUnit(const int unit) const;
	};

	class TextureManager
	{
	private:
		std::set<std::string> m_loadedPaths;
		std::map<unsigned int, Texture*> m_textures;
		unsigned int m_defaultTexId;

	public:
		TextureManager();

		unsigned int loadTexture(const std::string& path);
		void deleteTexture(const unsigned int id);
		const Texture* getTexture(const unsigned int id) const;
		const Texture* getDefaultTexture() const;
	};
}