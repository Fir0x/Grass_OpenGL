#pragma once

#include <GL/glew.h>
#include <vector>

namespace GLEngine
{
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		bool normalized;

		static unsigned int get_type_size(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:
				return sizeof(GLfloat);
			case GL_UNSIGNED_INT:
				return sizeof(GLuint);
			default:
				return 0;
			}
		}
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_elements;
		unsigned int m_stride = 0;

	public:
		const std::vector<VertexBufferElement>& get_elements() const
		{
			return m_elements;
		}

		const unsigned int get_stride() const
		{
			return m_stride;
		}

		template<typename T>
		void Add(unsigned int count) = delete;

		template<>
		void Add<float>(unsigned int count)
		{
			m_elements.push_back({ GL_FLOAT, count, false });
			m_stride += count * VertexBufferElement::get_type_size(GL_FLOAT);
		}

		template<>
		void Add<unsigned int>(unsigned int count)
		{
			m_elements.push_back({ GL_UNSIGNED_INT, count, false });
			m_stride += count * VertexBufferElement::get_type_size(GL_UNSIGNED_INT);
		}
	};
}

