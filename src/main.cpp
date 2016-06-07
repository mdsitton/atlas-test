
#include <iostream>
#include <stdexcept>
#include <string>
#include "application.hpp"
#include "renderer.hpp"
#include "util.hpp"
#include "texture.hpp"

#include "GL/gl3w.h"

void init_sdl()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw std::runtime_error(SDL_GetError());
    }
}

void init_gl3w()
{
    if (gl3wInit())
    {
        throw std::runtime_error("Failed to initialize opengl.");
    }
    if (!gl3wIsSupported(3, 3)) {
        throw std::runtime_error("OpenGL 3.3 not supported.");
    }
}

void run()
{
    init_sdl();
    Application atlasTest {800, 600, "Atlas Tests"};
    init_gl3w();
    Renderer renderer(800, 600);


    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    Texture testTex(blank_image(), renderer.get_shader_program());

    while (atlasTest.process_events())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        atlasTest.swap();
        renderer.error_check();
    }
}


int main(int argc, char const *argv[])
{
    // This is mainly is a global exception handler to show friendly messages to users.
    try {
        run();
    } catch (std::runtime_error &err) {
        std::cout << "Runtime Error:\n" << err.what() << std::endl;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Runtime Error", err.what(), nullptr);
        return 1;
    }

    return 0;
}
