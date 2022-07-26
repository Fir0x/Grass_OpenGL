#include "glError.h"

#include <GL/glew.h>
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    if (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << "): " << function
            << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}

bool GLLogCall(const char* file, int line)
{
    if (GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << "): undetected function "
            << file << ":" << line << std::endl;
        return false;
    }

    return true;
}
