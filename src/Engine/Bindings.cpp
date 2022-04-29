#include <Bindings.hpp>

// Default Constructor OwO
Bindings::Bindings(){}

// Default Deconstructor UwU
Bindings::~Bindings(){}

void Bindings::setFunc(uint8_t scancode, std::function<void()> func){
  __binds.insert(std::pair<uint8_t, std::function<void()>> (scancode, func) );
}

void Bindings::callFunc(uint8_t scancode){
  if (__binds.count(scancode) > 0){
    (__binds.at(scancode))();
  }
}


void Bindings::callUpFunc(uint8_t scancode, std::function<void()> func){
   SDL_PollEvent(&e);
        if (e.type == SDL_KEYDOWN){
            printf("Amogus2\n");
            if (e.key.keysym.scancode == scancode){
                printf("Amogus\n");
                func();
            }
        }

}
