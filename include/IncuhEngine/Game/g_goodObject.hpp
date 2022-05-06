#ifndef INCUHENGINE_G_GOODOBJECT_HPP
#define INCUHENGINE_G_GOODOBJECT_HPP
#include <Game/GameObject.hpp>
#include <Bindings.hpp>
#include <Model.hpp>
#include <SDL2/SDL_events.h>
namespace Incuh {
    class g_goodObject : public GameObject {
    public:
        g_goodObject() {}

        void start(){}
        void update() {
            for (unsigned int g = 0; g < models.size(); g++){
                if (Bindings::getKeyDown(SDL_SCANCODE_SPACE)) {
                    float direction[3];
                    direction[0] = 0.0f;
                    direction[1] = 0.0f;
                    direction[2] = 1.0f;
                    models.at(g)->getPhysicsMesh()->applyForce(direction, 5000.0f);
                }
            }
        }
    private:
    };
}

#endif //INCUHENGINE_G_GOODOBJECT_HPP
