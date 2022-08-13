#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

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

	struct Material
	{
		std::string name;
		Mesh::Color diffuse;
	};

	static std::map<std::string, Material> loadMtl(const std::string& path)
	{
		std::map<std::string, Material> lib;

		std::ifstream stream;
		stream.open(path);
		bool pushMtl = false;
		Material currentMtl;

		if (stream.is_open())
		{
			std::string line;
			while (std::getline(stream, line))
			{
				if (line[0] == '#' || line.empty())
					continue;

				auto splitted = splitstr(line, ' ');
				const std::string& tag = splitted[0];
				if (tag == "newmtl")
				{
					if (pushMtl)
					{
						lib.insert({ currentMtl.name, currentMtl });
						currentMtl = {};
					}
					else
						pushMtl = true;

					currentMtl.name = splitted[1];
				}
				else if (tag == "Kd")
				{
					currentMtl.diffuse.r = std::stof(splitted[1]);
					currentMtl.diffuse.g = std::stof(splitted[2]);
					currentMtl.diffuse.b = std::stof(splitted[3]);
				}
			}

			lib.insert({ currentMtl.name, currentMtl });
		}

		return lib;
	}

	std::vector<Mesh> Mesh::loadOBJFile(const std::string& path)
	{
		std::ifstream stream;
		stream.open(path);

		std::vector<Mesh> meshes;

		if (stream.is_open())
		{
			Mesh currentMesh;

			std::vector<Mesh::Vertex> vertices;
			std::vector<Mesh::UV> uvs;
			std::vector<Mesh::Normal> normals;
			std::map<int, std::map<int, int>> indexMapper;
			std::map<std::string, Material> mtllib;
			bool pushMesh = false;
			bool useMtl = false;
			Material currentMtl;

			std::string folder = path.substr(0, path.find_last_of('\\') + 1);
			std::string line;
			while (std::getline(stream, line))
			{
				if (line[0] == '#')
					continue;

				auto splitted = splitstr(line, ' ');
				if (line[0] == 'o')
				{
					if (pushMesh)
					{
						indexMapper.clear();
						meshes.push_back(currentMesh);
						currentMesh = Mesh();
					}
					else
						pushMesh = true;
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
						if (indexMapper.find(vertIdx) == indexMapper.end())
						{
							indexMapper.insert({ vertIdx, std::map<int, int>() });
						}

						if (indexMapper[vertIdx].find(normIdx) == indexMapper[vertIdx].end())
						{
							Mesh::Vertex v = vertices.at(vertIdx);
							Mesh::UV uv = uvs.at(uvIdx);
							Mesh::Normal n = normals.at(normIdx);
							if (useMtl)
							{
								Mesh::Color color = currentMtl.diffuse;
								indexMapper[vertIdx].insert({ normIdx, currentMesh.addFaceVertex(v, uv, n, color) });
							}
							else
							{
								indexMapper[vertIdx].insert({ normIdx, currentMesh.addFaceVertex(v, uv, n) });
							}
						}
						else
						{
							currentMesh.addFaceVertex(indexMapper[vertIdx][normIdx]);
						}
					}
				}
				else if (splitted[0] == "mtllib")
				{
					mtllib = loadMtl(folder + splitted[1]);
				}
				else if (splitted[0] == "usemtl")
				{
					currentMtl = mtllib[splitted[1]];
					useMtl = true;
				}
			}

			if (pushMesh)
			{
				meshes.push_back(currentMesh);
			}
		}
		else
		{
			std::cerr << "Failed to open OBJ file at " << path << "\n";
		}

		return meshes;
	}
}