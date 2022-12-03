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

		/// <summary>
		/// Allocate an interleaved buffer to pass to a VBO.
		/// </summary>
		/// <returns></returns>
		float* generateBuffer(size_t &size) const;

		size_t triangleCount() const;
		const std::vector<unsigned int>& getIndices() const;

		/// <summary>
		/// Write buffer representation as an OBJ file.
		/// Used to check if buffer is well encoded.
		/// </summary>
		/// <param name="path"></param>
		void writeBufferAsOBJ(const char* path);

		static std::optional<StaticMesh> loadOBJFile(const std::string& path);
	};
}