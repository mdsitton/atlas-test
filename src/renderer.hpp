#pragma once
#include <string>
#include <glm/glm.hpp>

#include "GL/gl3w.h"

GLuint create_shader(std::string shader, GLenum shaderType);
GLuint setup_program(GLuint vertex, GLuint fragment);
void check_shader(GLuint shader);
void check_program(GLuint program);

/*
struct Rect
{
    // texture
    // modelmatrix
    // ShaderProgram
};
*/
class ObjectManager
{
public:
    uint add_object();
};

class Renderer
{
public:
    Renderer(int width, int height);
    void render();
    void error_check();
    GLuint get_shader_program() {return m_program;}
private:
    GLuint m_vao;
    GLuint m_program;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;

    GLint m_vertLoc;
    GLint m_texCoordLoc;
    GLint m_groupingLoc;

    GLuint m_vertBuffer;
    GLuint m_texCoordBuffer;
    GLuint m_groupingBuffer;



    int m_width;
    int m_height;
    glm::mat4 m_projection;
};
