#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
#include "IDrawable.h"
#include "MeshRenderer.h"
#include "Transform.h"

namespace GLEngine
{
	class Object : IDrawable
	{
	private:
		Transform m_transform;
		MeshRenderer* m_renderer;


		glm::mat3 processNormalMatrix(const glm::mat4& model, const glm::mat4& view) const;

	public:
		Object(MeshRenderer* renderer);
		Object(const glm::vec3& pos, MeshRenderer* renderer);
		~Object();

		Transform& getTransform();

		virtual void draw(const DrawContext& context) const override;
	};
}