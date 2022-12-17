#include "InputManager.h"

#include <glm/glm.hpp>

static float lastFrameTime = 0;
static bool mouseCamera = true;
static bool firstFocus = true;
static GLEngine::Camera* mainCamera;

void linkCamera(GLEngine::Camera* camera)
{
    mainCamera = camera;
}

void processInput(GLFWwindow* window)
{
    static float speed = 2.5f;
    static glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    float currentFrameTime = (float)glfwGetTime();
    float deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    // GLFW keys match position on a WASD layout,
    // not the actual keyboard layout.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCamera->translate(mainCamera->forward() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera->translate(-mainCamera->forward() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera->translate(mainCamera->right() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCamera->translate(-mainCamera->right() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        mainCamera->translate(worldUp * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        mainCamera->translate(-worldUp * speed * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (!mouseCamera)
            firstFocus = true;

        mouseCamera = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        mouseCamera = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

static double lastX;
static double lastY;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!mouseCamera)
        return;

    if (firstFocus)
    {
        lastX = xpos;
        lastY = ypos;
        firstFocus = false;
    }

    float xoffset = (float)(xpos - lastX);
    float yoffset = (float)(lastY - ypos);
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.07f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    mainCamera->rotate(yoffset, xoffset);
}