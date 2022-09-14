#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <vector>

#include "Texture.h"

namespace GLEngine
{
	class Mesh
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
		/// <summary>
		/// Add a new vertex to the mesh.
		/// </summary>
		/// <param name="coords"></param>
		/// <param name="normals"></param>
		/// <param name="uvs"></param>
		/// <returns>Index of the created vertex.</returns>
		int addFaceVertex(glm::vec3 coords, glm::vec3 normals, glm::vec2 uvs);

		/// <summary>
		/// Reuse a vertex for a new face.
		/// </summary>
		/// <param name="index">Vertex index</param>
		void addFaceVertex(int index);

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

		static std::optional<Mesh> loadOBJFile(const std::string& path);
	};
}