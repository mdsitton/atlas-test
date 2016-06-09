
#include <iostream>
#include "texture.hpp"
#include "util.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
//exclude image formats
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STBI_NO_PSD
#define STBI_NO_GIF
// This leaves us with the following supported image formats:
// jpg  png  bmp  tga  hdr
#include <stb_image.h>

static int _texCount = 0;

Image load_image(std::string filename)
{
    std::string mem_buf = read_file( filename, FileMode::Binary );
    Image imgData;
    imgData.path = filename;

    auto conv_mem = std::make_unique<unsigned char[]>(mem_buf.size());

    // to prevent potential issues convert each value seperately
    // one could cast the int* to unsigned int* however this could have large issues there can be
    // platform differances on how this is imeplemented.
    for (size_t i = 0; i < mem_buf.size(); i++) {
        conv_mem[i] = static_cast<unsigned char>(mem_buf[i]);
    }

    unsigned char *img_buf;

    img_buf = stbi_load_from_memory(conv_mem.get(), mem_buf.size(), &imgData.width, &imgData.height, &imgData.pxComponents, 0);

    if ( img_buf == nullptr )
        throw "Failed to get image data";

    auto &pxData = imgData.pixelData;
    pxData.reserve(imgData.width*imgData.height*4);


    // We have to copy the data out of the returned data from stb.
    // This is being done manually so that we only have to deal with RGBA data later.
    int i, j;
    for (int x = 0;x < imgData.width; x++) {
        for (int y = 0; y < imgData.height; y++) {
            i = 4 * (y * imgData.width + x);
            j = imgData.pxComponents * (y * imgData.width + x);
            pxData[i+0] = img_buf[j+0];
            pxData[i+1] = img_buf[j+1];
            pxData[i+2] = img_buf[j+2];
            if (imgData.pxComponents == 4) {
                pxData[i+3] = img_buf[j+3];
            } else {
                pxData[i+3] = 255U;
            }
        }
    }
    stbi_image_free( img_buf );
    return imgData;
}

// Fake load image to test image loading.
Image blank_image(int width, int height)
{
    Image imgData;
    imgData.width = width;
    imgData.height = height;
    imgData.pxComponents = 4;
    int length = imgData.width*imgData.height*imgData.pxComponents;

    auto &pxData = imgData.pixelData;
    pxData.reserve(length);

    for (int i = 0; i < length; i++) {
        pxData[i] = 255U;
    }
    return imgData;
}



// Basic Texture class

Texture::Texture(Image image, GLuint program)
: m_image(image), m_program(program), m_texUnitID(_texCount)
{
    _texCount++;

    m_texSampID = glGetUniformLocation(m_program, "textureSampler");

    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_image.width, m_image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &m_image.pixelData[0]);
}

void Texture::bind()
{
    glActiveTexture(GL_TEXTURE0+m_texUnitID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glUniform1i(m_texSampID, m_texUnitID);
    std::cout << "Texture bound" << std::endl;
}

// Texture Atlas class
TextureGridAtlas::TextureGridAtlas(GLuint program, int texSize, int gridSize)
:m_program(program), m_size(texSize), m_gridSize(gridSize), m_texUnitID(_texCount)
{
    _texCount++;

    m_texSampID = glGetUniformLocation(m_program, "textureSampler");

    glGenTextures(1, &m_texID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size, m_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

AtlasTexID TextureGridAtlas::add_texture(Image image)
{
    auto id = static_cast<AtlasTexID>(m_textures.size());
    m_textures.push_back(image);
    return id;
}

BoundingBox TextureGridAtlas::get_uvcoords(AtlasTexID tex)
{
    BoundingBox &points = m_texRect[tex];

    double x1 = points.one.x / m_size;
    double y1 = points.one.y / m_size;
    double x2 = points.two.x / m_size;
    double y2 = points.two.y / m_size;

    return {x1, y1, x2, y2};
}

void TextureGridAtlas::generate()
{
    m_texRect.resize(m_textures.size());

    int cursorPosX = 0;
    int cursorPosY = 0;

    int maxCursor = m_size - m_gridSize;

    for(auto i = m_textures.begin(); i != m_textures.end(); ++i)
    {
        Image &texture = *i;
        BoundingBox &rect = m_texRect[std::distance(m_textures.begin(), i)];

        rect.one.x = cursorPosX;
        rect.one.y = cursorPosY;
        rect.two.x = cursorPosX + m_gridSize;
        rect.two.y = cursorPosY + m_gridSize;

        glTexSubImage2D(GL_TEXTURE_2D, 0, cursorPosX, cursorPosY, texture.width, texture.height, GL_RGBA, GL_UNSIGNED_BYTE, &texture.pixelData[0]);

        // once at the max cursor start the next line in the atlas.
        if (cursorPosX > maxCursor) {
            cursorPosX = 0;
            cursorPosY += m_gridSize;
        } else {
            cursorPosX += m_gridSize;
        }

        // break out if we are past the max cursor position
        if (cursorPosY > maxCursor) {
            break;
        }
    }
}

void TextureGridAtlas::bind()
{
    glActiveTexture(GL_TEXTURE0+m_texUnitID);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glUniform1i(m_texSampID, m_texUnitID);
    std::cout << "TextureAtlas bound" << std::endl;
}
