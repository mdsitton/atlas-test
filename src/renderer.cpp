#include <memory>
#include <iostream>
#include <stdexcept>

#include "renderer.hpp"

GLuint create_shader(std::string shaderStr, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    const char *c_str = shaderStr.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        auto logData = std::make_unique<GLchar[]>(length+1);
        glGetShaderInfoLog(shader, length, nullptr, logData.get());
        logData[length] = '\0';
        std::string str = "Shader compilation error.\n";
        str.append(logData.get());
        throw std::runtime_error(str.c_str());
    } else {
        std::cout << "Shader compiled successfully." << std::endl;
    }

    return shader;
}
