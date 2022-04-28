#ifndef UI_RENDER_HPP
#define UI_RENDER_HPP
#include <SDL2/SDL.h>
#include <string>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>


class UI_Render
{
    public:
        UI_Render(SDL_Window *win, SDL_GLContext con);
        virtual ~UI_Render();

        void updateEvents(SDL_Event *e);
        void newFrame();
        void render();

        // Render functions:
        void beginWindow(std::string title, bool *target){
            ImGui::Begin(title.c_str(), target, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar);
        }

        void endWindow(){
            ImGui::End();
        }



    protected:

    private:
        //SDL_Texture *uTarget;
        SDL_Window *uWin;
        SDL_GLContext uContext;
};

#endif // UI_RENDER_HPP
