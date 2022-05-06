#ifndef BINDINGS_H
#define BINDINGS_H
#ifndef NOEXERNLIBS
#include <SDL2/SDL.h>
#include <fstream>
#include <functional>
#include <vector>
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

        static std::map<int16_t, std::function<void()> > __upbinds;
        static std::map<int16_t, std::function<void()> > __downbinds;
        static std::vector<int64_t> keyUpState;
        static std::vector<int64_t> keyState;
        static std::vector<int64_t> keyDownState;

        static void resetState();
        static int getKeyDown(uint8_t scancode);
        static int getKeyUp(uint8_t scancode);
        static int getKey(uint8_t scancode);

        static void registerKeyUp(uint8_t scancode);
        static void registerKey(uint8_t scancode);
        static void registerKeyDown(uint8_t scancode);

        static void setUpFunc(uint8_t scancode,std::function<void()> func);
        static void setDownFunc(uint8_t scancode,std::function<void()> func);
        static void callBackDown(uint8_t scancode);
        static void callBackUp(uint8_t scancode);
    private:
};

#endif
