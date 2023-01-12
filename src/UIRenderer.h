#pragma once

#include <GLFW/glfw3.h>

namespace GLEngine
{
	class UIRenderer
	{
	private:
		GLFWwindow* m_window = nullptr;

	public:
		UIRenderer(GLFWwindow* window);
		~UIRenderer();

		void start();
		void finish();

		bool forwardMouseEvent(int button, bool down) const;
	};
}