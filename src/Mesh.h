#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace GLEngine
{
	class Mesh
	{
	public:
		struct Vertex
		{
			float x;
			float y;
			float z;
		};

		struct UV
		{
			float u;
			float v;
		};

		struct Normal
		{
			float x;
			float y;
			float z;
		};

		struct Color
		{
			float r;
			float g;
			float b;
		};

	private:
		std::vector<Vertex> m_vertices;
		std::vector<UV> m_uvs;
		std::vector<Normal> m_normals;
		std::vector<Color> m_colors;
		std::vector<unsigned int> m_indices;

	public:
		/// <summary>
		/// Add a new vertex to the mesh.
		/// </summary>
		/// <param name="vertex"></param>
		/// <param name="uv"></param>
		/// <param name="normal"></param>
		/// <param name="color"></param>
		/// <returns>Index of the created vertex.</returns>
		int addFaceVertex(Vertex vertex, UV uv, Normal normal, Color color = { 1.0f, 1.0f, 1.0f });

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
	};
}