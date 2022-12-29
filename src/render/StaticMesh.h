#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <vector>

#include "Texture.h"

namespace GLEngine
{
	class StaticMesh
	{
	public:
		struct Vertex
		{
			glm::vec3 coords;
			glm::vec3 normals;
			glm::vec2 uvs;
		};

	private:
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;

	public:
		StaticMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

		const std::vector<Vertex>& getData() const;

		size_t triangleCount() const;
		const std::vector<unsigned int>& getIndices() const;

		static std::optional<StaticMesh> loadOBJFile(const std::string& path);
	};
}