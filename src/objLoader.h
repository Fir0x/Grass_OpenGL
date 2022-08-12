#pragma once

#include <string>
#include <vector>

#include "Mesh.h"

namespace GLEngine
{
	std::vector<Mesh> loadOBJ(const std::string &path);
}