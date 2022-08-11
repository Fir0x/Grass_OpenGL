#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>

#include "Shader.h"
#include "glError.h"
#include "objLoader.h"
#include "Camera.h"
#include "Object.h"

static float lastFrameTime = 0;
static GLEngine::Camera mainCamera;

static void processInput(GLFWwindow* window)
{
    static float speed = 2.5f;
    static glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    float currentFrameTime = glfwGetTime();
    float deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    // GLFW keys match position on a WASD layout,
    // not the actual keyboard layout.
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mainCamera.translate(mainCamera.forward() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera.translate(-mainCamera.forward() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera.translate(mainCamera.right() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mainCamera.translate(-mainCamera.right() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        mainCamera.translate(worldUp * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        mainCamera.translate(-worldUp * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

static bool firstFocus = true;
static double lastX;
static double lastY;

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstFocus)
    {
        lastX = xpos;
        lastY = ypos;
        firstFocus = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.07f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    mainCamera.rotate(yoffset, xoffset);
}

int main(void)
{
    GLFWwindow* window;

    // Initialize the library
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to init GLEW" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, 800, 600);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glClearColor(0.4, 0.4, 0.4, 1.0);

    {
        glfwSetCursorPosCallback(window, mouse_callback);

        mainCamera = GLEngine::Camera(glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f),
            glm::vec3(0.0f, 0.0f, 5.0f));

        std::vector<GLEngine::Object*> toRender;

        auto meshes = GLEngine::loadOBJ("meshes\\cube.obj");
        for (const auto& mesh : meshes)
        {
            toRender.push_back(new GLEngine::Object(mesh));
        }

        GLEngine::Shader shader("base.glsl");
        shader.use();

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            shader.setUniformMatrix4f("projectionMatrix", mainCamera.getProjectionMatrix());
            shader.setUniformMatrix4f("viewMatrix", mainCamera.getViewMatrix());

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (const auto& obj : toRender)
            {
                obj->draw();
            }

            glfwSwapBuffers(window);

            glfwPollEvents();
        }

        for (const auto& obj : toRender)
        {
            delete obj;
        }
    }

    glfwTerminate();
    return 0;
}