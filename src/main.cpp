#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>

#include "render/Program.h"
#include "glWrappers/glError.h"
#include "scene/Camera.h"
#include "scene/objects/Object.h"
#include "render/Texture.h"
#include "scene/lights/DirectionalLight.h"
#include "scene/lights/PointLight.h"
#include "scene/lights/SpotLight.h"
#include "InputManager.h"

static GLEngine::Camera mainCamera;

static struct Settings
{
    unsigned int screen_width = 800;
    unsigned int screen_height = 600;
} settings;

static void screen_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    mainCamera.onScreenSizeChange(width, height);
    settings.screen_width = width;
    settings.screen_height = height;
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
    window = glfwCreateWindow(settings.screen_width, settings.screen_height, "GLEngine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to init GLEW" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, settings.screen_width, settings.screen_height);

    linkCamera(&mainCamera);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    {
        glfwSetCursorPosCallback(window, mouse_callback);

        float aspectRatio = (float) settings.screen_width / settings.screen_height;
        mainCamera = GLEngine::Camera(glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f),
            glm::vec3(0.0f, 0.0f, 5.0f));

        glfwSetFramebufferSizeCallback(window, screen_size_callback);

        std::vector<GLEngine::Object*> toRender;

        auto baseProgram = GLEngine::Program::fromFiles("shaders\\core\\base.vert", "shaders\\core\\base.frag");
        auto computeProgram = GLEngine::Program::fromFiles("shaders\\core\\genPlane.comp");
        auto grassProgram = GLEngine::Program::fromFiles("shaders\\core\\grass.vert", "shaders\\core\\grass.geom", "shaders\\core\\grass.frag");
        auto debugProgram = GLEngine::Program::fromFiles("shaders\\debug\\debugPlane.vert", "shaders\\debug\\debugPlane.geom", "shaders\\debug\\debugPlane.frag");

        struct FrameContext
        {
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
        };

        GLEngine::TypedBuffer<float> grassPlane(1024 * 8);

        GLEngine::VertexBufferLayout layout;
        layout.Add<float>(2);
        layout.Add<float>(2);
        layout.Add<float>(3);
        layout.Add<float>(1);

        auto grassVAO = GLEngine::VertexArray(grassPlane, layout);
        glm::mat4 identityModel = glm::mat4(1.0f);

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            FrameContext context = { mainCamera.getViewMatrix(), mainCamera.getProjectionMatrix() };
            GLEngine::TypedBuffer<FrameContext> contextBuffer(&context, 1);
            contextBuffer.bind<GLEngine::BufferUsageType::UniformBuffer>(0);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            computeProgram->use();
            grassPlane.bind<GLEngine::BufferUsageType::ShaderStorage>(0);
            GL_CALL(glDispatchCompute(1, 1, 1));
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            grassProgram->use();
            grassProgram->setUniform("modelMatrix", identityModel);
            grassVAO.bind();
            GL_CALL(glDrawArrays(GL_POINTS, 0, 1024));

            // Update step
            for (const auto& obj : toRender)
            {
                obj->update();
            }

            baseProgram->use();

            // Draw step
            for (const auto& obj : toRender)
            {
                obj->draw({ *baseProgram, mainCamera.getViewMatrix() });
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