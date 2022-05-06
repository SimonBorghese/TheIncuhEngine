#include <Bindings.hpp>

std::map<int16_t, std::function<void()> > Bindings::__upbinds = {};
std::map<int16_t, std::function<void()> > Bindings::__downbinds = {};

std::vector<int64_t> Bindings::keyUpState = {};
std::vector<int64_t> Bindings::keyState = {};
std::vector<int64_t> Bindings::keyDownState = {};


void Bindings::resetState(){
     Bindings::keyUpState.clear();
     Bindings::keyState.clear();
     Bindings::keyDownState.clear();
}
int Bindings::getKeyDown(uint8_t scancode){
    return std::count(Bindings::keyDownState.begin(), Bindings::keyDownState.end(), scancode) > 0;
}
int Bindings::getKeyUp(uint8_t scancode){
    return std::count(Bindings::keyUpState.begin(), Bindings::keyUpState.end(), scancode) > 0;
}
int Bindings::getKey(uint8_t scancode){
    return std::count(Bindings::keyState.begin(), Bindings::keyState.end(), scancode) > 0;
}

void Bindings::registerKeyUp(uint8_t scancode){
    Bindings::keyUpState.push_back(scancode);
}
void Bindings::registerKey(uint8_t scancode){
    Bindings::keyState.push_back(scancode);
}
void Bindings::registerKeyDown(uint8_t scancode){
    Bindings::keyDownState.push_back(scancode);
}

void Bindings::setUpFunc(uint8_t scancode,std::function<void()> func){
    Bindings::__upbinds.insert(std::pair<uint8_t, std::function<void()>> (scancode, func) );
}
void Bindings::setDownFunc(uint8_t scancode,std::function<void()> func){
    Bindings::__downbinds.insert(std::pair<uint8_t, std::function<void()>> (scancode, func) );
}
void Bindings::callBackDown(uint8_t scancode){
    if (Bindings::__downbinds.count(scancode) > 0){
        (Bindings::__downbinds.at(scancode))();
    }
}
void Bindings::callBackUp(uint8_t scancode){
    if (Bindings::__upbinds.count(scancode) > 0){
        (Bindings::__upbinds.at(scancode))();
    }
}