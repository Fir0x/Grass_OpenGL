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
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    {
        glfwSetCursorPosCallback(window, mouse_callback);

        float aspectRatio = (float) settings.screen_width / settings.screen_height;
        mainCamera = GLEngine::Camera(glm::perspective(45.0f, aspectRatio, 0.1f, 100.0f),
            glm::vec3(0.0f, 0.0f, 5.0f));

        glfwSetFramebufferSizeCallback(window, screen_size_callback);

        std::vector<GLEngine::Object*> toRender;
        auto mesh = GLEngine::StaticMesh::loadOBJFile("meshes\\cylinder.obj");
        if (!mesh.has_value())
            return 1;

        unsigned int matId = GLEngine::MaterialLibrary::loadFromMtl("meshes\\cylinder.mtl");

        auto renderer = new GLEngine::MeshRenderer(mesh.value());
        renderer->setMaterial(matId);

        auto* object = new GLEngine::Object(renderer);
        GLEngine::QuickBehavior bhv([](GLEngine::Object* obj) {
                obj->getTransform().rotate(0.01f, 0.01f, 0.01f);
            });
        object->setBehavior(&bhv);
        object->getTransform().scale(0.5f);
        toRender.push_back(object);

        auto baseProgram = GLEngine::Program::fromFiles("shaders\\core\\base.vert", "shaders\\core\\base.frag");
        auto test = GLEngine::Program::fromFiles("shaders\\debug\\debugPlane.vert", "shaders\\debug\\debugPlane.geom", "shaders\\debug\\debugPlane.frag");
        glm::vec3 white(1.0f);
        GLEngine::DirectionalLight dirLight(white, glm::vec3(0.0f, -1.0f, 0.0f));
        GLEngine::PointLight pointLight1(white, glm::vec3(3.0f, 0.0f, 0.0f), 100);
        GLEngine::PointLight pointLight2(white, glm::vec3(-3.0f, 0.0f, 0.0f), 100);
        GLEngine::SpotLight spotLight(white, glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 25.0f, 35.0f, 100);

        struct FrameContext
        {
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
        };

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            FrameContext context = { mainCamera.getViewMatrix(), mainCamera.getProjectionMatrix() };
            GLEngine::TypedBuffer<FrameContext> contextBuffer(&context, 1);
            contextBuffer.bind(0);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Update step
            for (const auto& obj : toRender)
            {
                obj->update();
            }

            baseProgram->use();
            //dirLight.SetupShaderProperties(shader, 0);
            pointLight1.SetupShaderProperties(*baseProgram, 0);
            pointLight2.SetupShaderProperties(*baseProgram, 1);
            //spotLight.SetupShaderProperties(shader, 0);

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