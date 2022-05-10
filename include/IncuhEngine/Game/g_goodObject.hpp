#ifndef INCUHENGINE_G_GOODOBJECT_HPP
#define INCUHENGINE_G_GOODOBJECT_HPP
#include <Game/GameObject.hpp>
#include <Bindings.hpp>
#include <Model.hpp>
#include <SDL2/SDL_events.h>
namespace Incuh {
    class g_goodObject : public GameObject {
    public:
        g_goodObject(IncuhState *state) : GameObject(state) {}

        void start(Model*){}
        void update(Model *model) {
            //if (Bindings::getKey(SDL_SCANCODE_B)) {

                float direction[3];

                direction[0] = mainState->mainCamera->getPos().x- model->getPosition().x;
                direction[1] = 0.0f;
                direction[2] = mainState->mainCamera->getPos().z- model->getPosition().z;
                model->getPhysicsMesh()->applyForce(direction, 0.4f);
            //}
        }
    private:
    };
}

#endif //INCUHENGINE_G_GOODOBJECT_HPP
