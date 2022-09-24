#pragma once

#include <functional>

#include "Object.h"

namespace GLEngine
{
	class Object;

	class Behavior
	{
	protected:
		Object* m_object;

	public:
		template <class T>
		static T* generateLinkedBehavior(T* behavior, Object* object)
		{
			static_assert(std::is_base_of_v<Behavior, T>);

			Behavior* linked = static_cast<Behavior*>(behavior);
			linked->m_object = object;

			return static_cast<T*>(linked);
		}

		virtual void update() = 0;
	};

	class QuickBehavior : public Behavior
	{
	private:
		const std::function<void(Object*)> m_behavior;

	public:
		QuickBehavior(std::function<void(Object*)> behavior)
			: m_behavior(behavior)
		{}

		virtual void update()
		{
			m_behavior(m_object);
		}
	};
}