
#pragma once
#include <string>
#include <memory>
#include <string>
#include <memory>
#include <vector>
#include "GL/gl3w.h"

struct Image
{
    int width;
    int height;
    int pxComponents;
    std::vector<unsigned char> pixelData;
    std::string path;
};

Image load_image(std::string filename);
Image blank_image();

class Texture
{
private:
    Image m_image;
    GLuint m_program;
    int m_texSampID;
    int m_texUnitID;
    GLuint m_texID;

public:
    Texture(Image image, GLuint program);
    void bind();
    //~Texture();
};

class TextureAtlas
{
private:
    std::vector<Image> test;
};
