#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene/Camera.h"

void linkCamera(GLEngine::Camera* camera);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);