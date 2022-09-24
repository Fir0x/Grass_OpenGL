#pragma once

#include "Shader.h"

namespace GLEngine
{
	struct DrawContext
	{
		Shader& currentShader;
		glm::mat4 viewMatrix;
	};

	class IDrawable
	{
	public:
		virtual void draw(const DrawContext& context) const = 0;
	};
}