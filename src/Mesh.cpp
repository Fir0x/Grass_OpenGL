#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "Material.h"

namespace GLEngine
{
	int Mesh::addFaceVertex(glm::vec3 coords, glm::vec3 normals, glm::vec2 uvs)
	{
		m_vertices.push_back({ coords, normals, uvs });

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

		Vertex* head = (Vertex*)buffer;
		for (int i = 0; i < m_vertices.size(); i++, head++)
		{
			*((Vertex*)head) = m_vertices[i];
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

	static std::vector<std::string> splitstr(const std::string& str, char delim)
	{
		std::vector<std::string> splitted;

		std::istringstream stream(str);
		std::string word;
		while (std::getline(stream, word, delim))
		{
			splitted.push_back(word);
		}

		return splitted;
	}

	std::optional<Mesh> Mesh::loadOBJFile(const std::string& path)
	{
		std::ifstream stream;
		stream.open(path);

		std::vector<Mesh> meshes;

		if (stream.is_open())
		{
			Mesh mesh;
			bool secondMesh = false;

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> normals;
			std::vector<glm::vec2> uvs;

			struct VertexInfo
			{
				int vertIdx;
				int normIdx;
				int uvIdx;

				bool operator<(const VertexInfo & rhs) const
				{
					return std::tie(vertIdx, normIdx, uvIdx) < std::tie(rhs.vertIdx, rhs.normIdx, rhs.uvIdx);
				}
			};
			std::map<VertexInfo, int> indexMapper;

			std::string folder = path.substr(0, path.find_last_of('\\') + 1);
			std::string line;
			while (std::getline(stream, line))
			{
				if (line[0] == '#')
					continue;

				auto splitted = splitstr(line, ' ');
				if (line[0] == 'o')
				{
					if (secondMesh)
					{
						std::cerr << "WARNING: " << path
							<< " contained more than one mesh definition. "
							<< "Only the first one was parsed.";
						return mesh;
					}
				}
				else if (line[0] == 'v')
				{
					if (line[1] == 't')
					{
						uvs.push_back({
							std::stof(splitted[1]),
							std::stof(splitted[2])
							});
					}
					else if (line[1] == 'n')
					{
						normals.push_back({
							std::stof(splitted[1]),
							std::stof(splitted[2]),
							std::stof(splitted[3])
							});
					}
					else
					{
						vertices.push_back({
							std::stof(splitted[1]),
							std::stof(splitted[2]),
							std::stof(splitted[3])
							});
					}
				}
				else if (line[0] == 'f')
				{
					for (int i = 1; i < 4; i++)
					{
						auto indices = splitstr(splitted[i], '/');
						int vertIdx = std::stoi(indices[0]) - 1;
						int uvIdx = std::stoi(indices[1]) - 1;
						int normIdx = std::stoi(indices[2]) - 1;
						VertexInfo vInfo = { vertIdx, uvIdx, normIdx };

						if (indexMapper.find(vInfo) == indexMapper.end())
						{
							glm::vec3 v = vertices.at(vertIdx);
							glm::vec3 n = normals.at(normIdx);
							glm::vec2 uv = uvs.at(uvIdx);
							indexMapper.insert({ vInfo, mesh.addFaceVertex(v, n, uv) });
						}
						else
						{
							mesh.addFaceVertex(indexMapper[vInfo]);
						}
					}
				}
			}

			return mesh;
		}
		else
		{
			std::cerr << "Failed to open OBJ file at " << path << "\n";
			return std::optional<Mesh>();
		}
	}
}