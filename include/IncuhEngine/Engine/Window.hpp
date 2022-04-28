#ifndef WINDOW_H
#define WINDOW_H
#include <GameState.h>
#include <Launcher/LauncherArgs.hpp>
#include <SDL2/SDL.h>
#include "glad/glad.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Overlay/UI_Render.hpp>
/*
Purpose of this code is to provide code for Window creation and key call back
*/
class Window{
  public:
    Window(IncuhState *state);
    virtual ~Window();

    void update();
    int getCloseState();

    std::vector<uint8_t> getActiveKeys();

    SDL_Window* getSDLWindow();
    SDL_GLContext getGLContext() { return __con; }

    void resetCursor(uint32_t newX = 0, uint32_t newY = 0);

    //void captureMouse();
    //void freeMouse();

    uint32_t getWidth();
    uint32_t getHeight();
  private:
    SDL_Window *__win;
    SDL_Event e;
    SDL_GLContext __con;
    uint32_t __width;
    uint32_t __height;

    const uint8_t *__keys;
    int __key_len;
    UI_Render *__mainUI;
};


#endif
