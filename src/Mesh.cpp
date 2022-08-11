#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

namespace GLEngine
{
	int Mesh::addFaceVertex(Vertex vertex, UV uv, Normal normal, Color color)
	{
		m_vertices.push_back(vertex);
		m_uvs.push_back(uv);
		m_normals.push_back(normal);
		m_colors.push_back(color);

		int index = (int)(m_vertices.size() - 1);
		m_indices.push_back(index);

		return index;
	}

	void Mesh::addFaceVertex(int index)
	{
		m_indices.push_back(index);
	}

	float* Mesh::generateBuffer(size_t &size) const
	{
		size = m_vertices.size() * sizeof(float) * 11;
		float* buffer = (float*)malloc(size);
		if (buffer == nullptr)
			return nullptr;

		float* head = buffer;
		for (int i = 0; i < m_vertices.size(); i++, head += 11)
		{
			*((Vertex*)head) = m_vertices[i];
			*((UV*)(head + 3)) = m_uvs[i];
			*((Normal*)(head + 5)) = m_normals[i];
			*((Color*)(head + 8)) = m_colors[i];
		}

		return buffer;
	}

	size_t Mesh::triangleCount() const
	{
		return m_indices.size() / 3;
	}

	const std::vector<unsigned int>& Mesh::getIndices() const
	{
		return m_indices;
	}

	void Mesh::writeBufferAsOBJ(const char* path)
	{
		std::ofstream stream;
		stream.open(path);

		if (!stream.is_open())
		{
			std::cerr << "Failed to write object as OBJ file at " << path << std::endl;
			return;
		}

		size_t bufferSize;
		float* buffer = generateBuffer(bufferSize);
		size_t count = bufferSize / sizeof(float);

		for (size_t i = 0; i < count; i += 8)
		{
			stream << "v " << buffer[i] << " " << buffer[i + 1] << " " << buffer[i + 2] << "\n";
		}

		for (size_t i = 0; i < count; i += 8)
		{
			stream << "vt " << buffer[i + 3] << " " << buffer[i + 4] << "\n";
		}

		for (size_t i = 0; i < count; i += 8)
		{
			stream << "vn " << buffer[i + 5] << " " << buffer[i + 6] << " " << buffer[i + 7] << "\n";
		}

		for (size_t i = 0; i < m_indices.size(); i += 3)
		{
			stream << "f";
			for (size_t j = 0; j < 3; j++)
			{
				auto idx = m_indices[i + j] + 1;
				stream << " " << idx << "/" << idx << "/" << idx;
			}
			stream << "\n";
		}

		stream.close();

		free(buffer);
	}
}