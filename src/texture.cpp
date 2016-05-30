
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
#include "stb_image.h"

static int _texCount = 0;
static GLuint _currentBoundtexture = 0;

Texture::Texture(std::string path, GLuint program)
: m_path(path), m_program(program)
{
    _texCount++;
    m_texUnitID = _texCount;

    load_image(m_path);

    m_texSampID = glGetUniformLocation(m_program, "textureSampler");

    GLuint texid;
    glGenTextures(1, &texid);
    m_texID = texid;
    glBindTexture(GL_TEXTURE_2D, m_texID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(m_pixelData.get())[0]);
}

void Texture::load_image(std::string filename)
{
    std::string mem_buf = read_file( filename, FileMode::Binary );

    auto conv_mem = std::make_unique<unsigned char[]>( mem_buf.size() );

    // to prevent potential issues convert each value seperately
    // one could cast the int* to unsigned int* however this could have large issues there can be
    // platform differances on how this is imeplemented.
    for (int i = 0; i < mem_buf.size(); i++) {
        conv_mem[i] = static_cast<unsigned char>(mem_buf[i]);
    }

    unsigned char *img_buf;
    int comp;

    img_buf = stbi_load_from_memory( &conv_mem[0], mem_buf.size(), &m_width, &m_height, &comp, 0 );

    if ( img_buf == nullptr )
        throw "Failed to get image data";

    m_pixelData = std::make_unique<unsigned char[]>(m_width*m_height*4);

    // We have to copy the data out of the returned data from stb.
    // This is being done manually so that we just have one format to deal with RGBA.
    int i, j;
    for (int x = 0;x < m_width; x++) {
        for (int y = 0; y < m_height; y++) {
            i = 4 * (y * m_width + x);
            j = comp * (y * m_width + x);
            m_pixelData[i+0] = img_buf[j+0];
            m_pixelData[i+1] = img_buf[j+1];
            m_pixelData[i+2] = img_buf[j+2];
            if (comp == 4) {
                m_pixelData[i+3] = img_buf[j+3];
            } else {
                m_pixelData[i+3] = 255U;
            }
        }
    }
    stbi_image_free( img_buf );
}

void Texture::bind()
{
    if (_currentBoundtexture != m_texUnitID) {
        _currentBoundtexture = m_texUnitID;

        glActiveTexture(GL_TEXTURE0+m_texUnitID);
        glBindTexture(GL_TEXTURE_2D, m_texID);
        glUniform1i(m_texSampID, m_texUnitID);
        std::cout << "Texture bound" << std::endl;

    }
}
