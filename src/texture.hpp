
#pragma once
#include <string>
#include <memory>
#include <string>
#include <memory>
#include "GL/gl3w.h"


class Texture
{
private:
    int m_width;
    int m_height;

    std::unique_ptr<unsigned char[]> m_pixelData;

    std::string m_path;
    GLuint m_program;
    int m_texSampID;
    int m_texUnitID;
    GLuint m_texID;

public:
    Texture(std::string path, GLuint program);
    void load_image(std::string filename);
    void bind();
    //~Texture();
};
