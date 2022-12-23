#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <memory>

namespace GLEngine
{
	class Texture
	{
	private:
		unsigned int m_id;
		glm::vec2 m_size;

	public:
		Texture(const glm::vec2& size);
		Texture(const glm::vec2& size, unsigned char* data);
		~Texture();

		static std::shared_ptr<Texture> fromFile(const std::string& path);

		unsigned int getId() const;

		void bind() const;
		void unbind() const;

		void bindToUnit(const int unit) const;
	};
}