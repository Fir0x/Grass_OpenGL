#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene/Camera.h"
#include "UIRenderer.h"

void linkCamera(GLEngine::Camera* camera);
void processInput(GLFWwindow* window, const GLEngine::UIRenderer& uiRenderer);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);