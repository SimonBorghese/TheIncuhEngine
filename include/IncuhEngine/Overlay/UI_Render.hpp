#ifndef UI_RENDER_HPP
#define UI_RENDER_HPP
#include <SDL2/SDL.h>
#include <Shader.hpp>



class UI_Render
{
    public:
        UI_Render();
        virtual ~UI_Render();

    protected:

    private:
        SDL_Texture *uTarget;
};

#endif // UI_RENDER_HPP
