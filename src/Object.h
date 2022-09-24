#pragma once

#include <glm/glm.hpp>

#include "Behavior.h"
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
		Behavior* m_behavior;


		glm::mat3 processNormalMatrix(const glm::mat4& model, const glm::mat4& view) const;

	public:
		Object(MeshRenderer* renderer);
		Object(const glm::vec3& pos, MeshRenderer* renderer);
		~Object();

		Transform& getTransform();

		template<class T>
		void setBehavior(T* behavior)
		{
			static_assert(std::is_base_of_v<Behavior, T>);
			m_behavior = Behavior::generateLinkedBehavior(behavior, this);
		}

		void update();
		virtual void draw(const DrawContext& context) const override;
	};
}