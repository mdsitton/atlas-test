
#pragma once
#include <string>
#include <memory>
#include <string>
#include <memory>
#include "GL/gl3w.h"

struct Image
{
    int width;
    int height;
    int pxComponents;
    std::unique_ptr<unsigned char[]> pixelData;
    std::string path;
};

Image load_image(std::string filename);
Image blank_image();

class Texture
{
private:
    const Image &m_image;
    GLuint m_program;
    int m_texSampID;
    int m_texUnitID;
    GLuint m_texID;

public:
    Texture(const Image &image, GLuint program);
    void bind();
    //~Texture();
};
