#pragma once

#include "render/Program.h"

namespace GLEngine
{
	struct DrawContext
	{
		Program& currentShader;
		glm::mat4 viewMatrix;
	};

	class IDrawable
	{
	public:
		virtual void draw(const DrawContext& context) const = 0;
	};
}