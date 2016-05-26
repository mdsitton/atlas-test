#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

GLuint create_shader(std::string shader, GLenum shaderType);

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
    Renderer();
    ~Renderer();
    void render();
private:
    GLuint m_vao;
    glm::mat4 m_projection;
};
