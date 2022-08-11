#include "objLoader.h"

#include <fstream>
#include <map>
#include <sstream>
#include <string>

namespace GLEngine
{
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

	std::vector<Mesh> loadOBJ(const char* path)
	{
		std::ifstream stream;
		stream.open(path);

		Mesh currentMesh;
		std::vector<Mesh> meshes;

		std::vector<Mesh::Vertex> vertices;
		std::vector<Mesh::UV> uvs;
		std::vector<Mesh::Normal> normals;
		std::map<int, std::map<int, int>> indexMapper;
		bool pushMesh = false;

		if (stream.is_open())
		{
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
					{
						pushMesh = true;
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
						if (indexMapper.find(vertIdx) == indexMapper.end())
						{
							indexMapper.insert({ vertIdx, std::map<int, int>() });
						}

						if (indexMapper[vertIdx].find(normIdx) == indexMapper[vertIdx].end())
						{
							Mesh::Vertex v = vertices.at(vertIdx);
							Mesh::UV uv = uvs.at(uvIdx);
							Mesh::Normal n = normals.at(normIdx);
							indexMapper[vertIdx].insert({ normIdx, currentMesh.addFaceVertex(v, uv, n) });
						}
						else
						{
							currentMesh.addFaceVertex(indexMapper[vertIdx][normIdx]);
						}
					}
				}
			}

			if (pushMesh)
			{
				meshes.push_back(currentMesh);
			}
		}

		return meshes;
	}
}