#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include "glad/glad.h"
#include <stdio.h>
#include <iostream>
#include <vector>
/*
Purpose of this code is to provide code for Window creation and key call back
*/
class Window{
  public:
    Window(const char *title, uint32_t width, uint32_t height);
    virtual ~Window();

    void update();
    int getCloseState();

    std::vector<uint8_t> getActiveKeys();

    SDL_Window* getSDLWindow();

    void resetCursor();

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
};


#endif
