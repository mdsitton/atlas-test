#include <memory>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer.hpp"
#include "util.hpp"


static const std::unordered_map<GLenum, std::string> oglErrorMap {
    {GL_INVALID_ENUM, "GL_INVALID_ENUM"},
    {GL_INVALID_VALUE, "GL_INVALID_VALUE"},
    {GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
    {GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"},
    {GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
    {GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
    {GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
    {GL_CONTEXT_LOST, "GL_CONTEXT_LOST"}
};

void check_shader(GLuint shader)
{
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
}

GLuint create_shader(std::string shaderPath, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    std::string fullShaderPath = expand_path(shaderPath);
    std::string shaderStr = read_file(fullShaderPath, FileMode::Normal);

    const char *c_str = shaderStr.c_str();
    glShaderSource(shader, 1, &c_str, nullptr);
    glCompileShader(shader);

    return shader;
}

void check_program(GLuint program)
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        auto logData = std::make_unique<GLchar[]>(length+1);
        glGetProgramInfoLog(program, length, nullptr, logData.get());
        logData[length] = '\0';
        std::string str = "Program linking error:\n";
        str.append(logData.get());
        throw std::runtime_error(str.c_str());
    } else {
        std::cout << "Program Linked successfully." << std::endl;
    }
}

GLuint setup_program(GLuint vertex, GLuint fragment)
{

    GLuint program = glCreateProgram();

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);

    glLinkProgram(program);

    return program;
}

Renderer::Renderer(int width, int height)
: m_width(width), m_height(height)
{
    m_projection = glm::ortho(0.0f, static_cast<float>(m_width), static_cast<float>(m_height), 0.0f, -1.0f, 1.0f);
    glViewport(0, 0, m_width, m_height);
    GLuint arr[1];
    glGenVertexArrays(1, arr);
    m_vao = arr[0];
    glBindVertexArray(m_vao);

    // we should check the shader compile status for shaders before we link the program
    m_vertexShader = create_shader("/data/shaders/main.vs", GL_VERTEX_SHADER);
    m_fragmentShader = create_shader("/data/shaders/main.fs", GL_FRAGMENT_SHADER);

    check_shader(m_vertexShader);
    check_shader(m_fragmentShader);

    m_program = setup_program(m_vertexShader, m_fragmentShader);

    check_program(m_program);


    glUseProgram(m_program);
}

void Renderer::error_check()
{
    GLenum err = glGetError();
    if(err != GL_NO_ERROR)
    {
        std::string errorStr = "The following opengl errors have occured:";

        while(err != GL_NO_ERROR)
        {
            errorStr.append("\n");
            errorStr.append(oglErrorMap.find(err)->second);
            err = glGetError();
        }
        throw std::runtime_error(errorStr.c_str());
    }
}

void Renderer::render()
{

}
