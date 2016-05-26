#pragma once
#include <string>

#include "SDL.h"
#undef main

class Application
{
public:
    Application(int width, int height, std::string title);
    ~Application();
    bool process_events();
    void swap();
private:
    int m_width;
    int m_height;
    std::string m_title;
    SDL_Window *m_window;
    SDL_GLContext m_context;
};
