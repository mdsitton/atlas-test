#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

GLuint create_shader(std::string shader, GLenum shaderType);
GLuint setup_program(GLuint vertex, GLuint fragment);
void check_shader(GLuint shader);
void check_program(GLuint program);

/*
struct Rect
{
    // texture
    // modelmatrix or x, y, rot, scale
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
    Renderer();
    ~Renderer();
    void render();
private:
    GLuint m_vao;
    glm::mat4 m_projection;
};
