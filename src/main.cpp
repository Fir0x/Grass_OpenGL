#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <iostream>

#include "Shader.h"
#include "glWrappers/glError.h"
#include "Camera.h"
#include "Object.h"
#include "Texture.h"
#include "lights/DirectionalLight.h"
#include "lights/PointLight.h"
#include "lights/SpotLight.h"

static float lastFrameTime = 0;
static GLEngine::Camera mainCamera;
static struct Settings
{
    unsigned int screen_width = 800;
    unsigned int screen_height = 600;
} settings;
static bool mouseCamera = true;
static bool firstFocus = true;

static void processInput(GLFWwindow* window)
{
    static float speed = 2.5f;
    static glm::vec3 worldUp(0.0f, 1.0f, 0.0f);

    float currentFrameTime = (float)glfwGetTime();
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

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

    mainCamera.rotate(yoffset, xoffset);
}

static void screen_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    mainCamera.onScreenSizeChange(width, height);
    settings.screen_width = width;
    settings.screen_height = height;
}

static glm::mat3 processNormalMatrix(const glm::mat4& model, const glm::mat4& view)
{
    return glm::mat3(glm::transpose(glm::inverse(view * model)));
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

    //std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glViewport(0, 0, settings.screen_width, settings.screen_height);

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
        GLEngine::TextureManager texManager;
        auto texId = texManager.loadTexture("textures\\texture.jpg");
        auto meshes = GLEngine::Mesh::loadOBJFile("meshes\\cylinder.obj", texManager);
        for (const auto& mesh : meshes)
        {
            toRender.push_back(new GLEngine::Object(mesh));
        }

        GLEngine::Shader shader("shaders\\core\\base.glsl");
        shader.use();
        glm::vec3 white(1.0f);
        GLEngine::DirectionalLight dirLight(white, glm::vec3(0.0f, -1.0f, 0.0f));
        GLEngine::PointLight pointLight1(white, glm::vec3(3.0f, 0.0f, 0.0f), 100);
        GLEngine::PointLight pointLight2(white, glm::vec3(-3.0f, 0.0f, 0.0f), 100);
        GLEngine::SpotLight spotLight(white, glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 25.0f, 35.0f, 100);

        while (!glfwWindowShouldClose(window))
        {
            processInput(window);

            shader.setUniformMatrix4f("projectionMatrix", mainCamera.getProjectionMatrix());
            shader.setUniformMatrix4f("viewMatrix", mainCamera.getViewMatrix());
            shader.setUniform3f("lightPos", 1.2f, 1.0f, 2.0f);
            shader.setUniform3f("lightColor", 1.0f, 1.0f, 1.0f);
            shader.setUniform1f("ambientStrength", 0.1f);
            shader.setUniform3f("viewPos", mainCamera.getPosition());

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //dirLight.SetupShaderProperties(shader, 0);
            pointLight1.SetupShaderProperties(shader, 0);
            pointLight2.SetupShaderProperties(shader, 1);
            //spotLight.SetupShaderProperties(shader, 0);

            for (const auto& obj : toRender)
            {
                shader.setUniformMatrix4f("modelMatrix", obj->getModelMatrix());
                shader.setUniformMatrix3f("normalMatrix", processNormalMatrix(obj->getModelMatrix(), mainCamera.getViewMatrix()));
                shader.setUniform3f("material.diffuse", white);
                shader.setUniform3f("material.specular", white);
                shader.setUniform1f("material.shininess", 32.0f);
                texManager.getTexture(texId)->bindToUnit(0);
                shader.setUniform1i("material.diffuseTex", 0);
                texManager.getDefaultTexture()->bindToUnit(1);
                shader.setUniform1i("material.specularTex", 1);
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