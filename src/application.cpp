#include <stdexcept>
#include "application.hpp"

Application::Application(int width, int height, std::string title)
: m_width(width), m_height(height), m_title(title)
{

    m_window = SDL_CreateWindow(m_title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            m_width, m_height, SDL_WINDOW_OPENGL);

    if (m_window == nullptr)
    {
        throw std::runtime_error("Could not create a window.");
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_context = SDL_GL_CreateContext(m_window);

    if (m_context == nullptr)
    {
        throw std::runtime_error("Could not create an OpenGL Context.");
    }

    if (SDL_GL_MakeCurrent(m_window, m_context) != 0)
    {
        throw std::runtime_error("Context MakeCurrent error.");
    }
}

Application::~Application()
{
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
}

void Application::swap()
{
    SDL_GL_SwapWindow(m_window);
}

bool Application::process_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {

        switch(event.type)
        {
            case SDL_QUIT:
                return false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                {
                    return false;
                }
                break;
            default:
                continue;
        }
    }
    return true;
}
