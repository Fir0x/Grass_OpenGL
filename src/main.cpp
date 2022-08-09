#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "glError.h"
#include "objLoader.h"
#include "Camera.h"



static float lastFrameTime = 0;
static GLEngine::Camera mainCamera;

static void processInput(GLFWwindow* window)
{
    static float speed = 2.5f;

    float currentFrameTime = glfwGetTime();
    float deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        mainCamera.translate(mainCamera.forward() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mainCamera.translate(-mainCamera.forward() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mainCamera.translate(mainCamera.right() * speed * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        mainCamera.translate(-mainCamera.right() * speed * deltaTime);
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

    float sensitivity = 0.1f;
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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    //glDepthRange(0.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glClearColor(0.4, 0.4, 0.4, 1.0);

    {
        glfwSetCursorPosCallback(window, mouse_callback);

        float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f    // top left 
        };
        unsigned int indices[] = {
            3, 1, 0,
            3, 2, 1
        };

        GLEngine::VertexBuffer vb(vertices, sizeof(vertices));
        GLEngine::VertexBufferLayout layout;
        layout.Add<float>(3);
        layout.Add<float>(3);

        GLEngine::VertexArray va(vb, layout);

        GLEngine::IndexBuffer ib(indices, 6);

        GLEngine::Shader shader("base.glsl");
        shader.use();

        auto meshes = GLEngine::loadOBJ("C:\\Users\\Firox\\Desktop\\cube.obj");

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            glClear(GL_COLOR_BUFFER_BIT);

            ib.bind();
            GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            glfwSwapBuffers(window);

            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}