#pragma once

#include <vector>

#include "Mesh.h"

namespace GLEngine
{
	std::vector<Mesh> loadOBJ(const char *path);
}