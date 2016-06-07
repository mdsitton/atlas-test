
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
Image blank_image(int width, int height);

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

typedef unsigned int AtlasTexID;

struct Coord
{
    double x;
    double y;
};

struct BoundingBox
{
    Coord one; // top left
    Coord two; // bottom right
};

class TextureGridAtlas
{
private:
    std::vector<Image> m_textures;
    std::vector<BoundingBox> m_texRect;
    GLuint m_program;
    int m_size;
    int m_gridSize;
    int m_texUnitID;
    int m_texSampID;
    GLuint m_texID;
public:
    TextureGridAtlas(GLuint program, int texSize=1024, int gridSize=64);
    AtlasTexID add_texture(Image image);
    std::array<Coord, 4> get_uvcoords(AtlasTexID tex);
    void generate();
    void bind();
};
