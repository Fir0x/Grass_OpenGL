#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

namespace GLEngine
{
	int Mesh::addFaceVertex(Vertex vertex, UV uv, Normal normal)
	{
		m_vertices.push_back(vertex);
		m_uvs.push_back(uv);
		m_normals.push_back(normal);

		int index = m_vertices.size() - 1;
		m_indices.push_back(index);

		return index;
	}

	void Mesh::addFaceVertex(int index)
	{
		m_indices.push_back(index);
	}

	float* Mesh::generateBuffer() const
	{
		float* buffer = (float*)malloc(m_vertices.size() * sizeof(float) * 8);
		Vertex* vertHead = (Vertex*)buffer;
		UV* uvHead = (UV*)(buffer + 3);
		Normal* normHead = (Normal*)(buffer + 5);
		for (int i = 0; i < m_vertices.size(); i++, vertHead++, uvHead++, normHead++)
		{
			*vertHead = m_vertices[i];
			*uvHead = m_uvs[i];
			*normHead = m_normals[i];
		}

		return buffer;
	}

	int Mesh::triangleCount()
	{
		return m_indices.size() / 3;
	}
}