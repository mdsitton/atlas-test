#pragma once
#include <string>
#include <unordered_map>

#include "GL/gl3w.h"
#include <glm/glm.hpp>

GLuint create_shader(std::string shader, GLenum shaderType);
GLuint setup_program(GLuint vertex, GLuint fragment);

//struct Rect
//{
    //texture
    //modelmat or pos/rot/scale x y rot scale
//};

class ObjectManager
{
public:
    uint add_object();
};

class Renderer
{
public:
    Renderer(int width, int height);
    //~Renderer();
    void render();
    GLuint get_shader_program() {return m_program;}
    void error_check();

private:
    GLuint m_vao;
    glm::mat4 m_projection;
    int m_width;
    int m_height;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
    GLuint m_program;
};
