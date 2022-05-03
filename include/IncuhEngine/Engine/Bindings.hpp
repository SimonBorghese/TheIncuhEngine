#ifndef BINDINGS_H
#define BINDINGS_H
#ifndef NOEXERNLIBS
#include <SDL2/SDL.h>
#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>
#endif
/*
PURPOSE:
This application should use a source engine-like binding system

HOW IT SHOULD FUNCTION:
A std::map should be created with a uint8_t and void (*func)
Every Frame, the keyboard state is recieved
Then each key is ititerated
If it has been pressed

Then we call the value at the key which is the SCANCODE of the key pressed
*/
class Bindings{
  public:
    Bindings();
    virtual ~Bindings();

    void setUpFunc(uint8_t scancode,std::function<void()> func);
    void setDownFunc(uint8_t scancode,std::function<void()> func);
    void callBackDown(uint8_t scancode);
    void callBackUp(uint8_t scancode);
  private:
    std::map<int16_t, std::function<void()> > __upbinds;
    std::map<int16_t, std::function<void()> > __downbinds;
};

#endif
