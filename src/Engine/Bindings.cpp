#include <Bindings.hpp>

// Default Constructor OwO
Bindings::Bindings(){}

// Default Deconstructor UwU
Bindings::~Bindings(){}


void Bindings::setUpFunc(uint8_t scancode,std::function<void()> func){
    __upbinds.insert(std::pair<uint8_t, std::function<void()>> (scancode, func) );
}
void Bindings::setDownFunc(uint8_t scancode,std::function<void()> func){
    __downbinds.insert(std::pair<uint8_t, std::function<void()>> (scancode, func) );
}
void Bindings::callBackDown(uint8_t scancode){
    if (__downbinds.count(scancode) > 0){
        (__downbinds.at(scancode))();
    }
}
void Bindings::callBackUp(uint8_t scancode){
    if (__upbinds.count(scancode) > 0){
        (__upbinds.at(scancode))();
    }
}