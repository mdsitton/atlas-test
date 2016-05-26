
#include <iostream>
#include <stdexcept>
#include <string>
#include "application.hpp"
#include <GL/glew.h>

void init_glew()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        throw std::runtime_error(reinterpret_cast<const char *>(glewGetErrorString(err)));
    }
}

int main(int argc, char const *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);

    try
    {
        Application atlasTest {800, 600, "Atlas Tests"};

        init_glew();
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        while (atlasTest.process_events())
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            atlasTest.swap();
        }

    } catch (std::runtime_error &err) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Runtime Error", err.what(), nullptr);
        return 1;
    }

    return 0;
}
