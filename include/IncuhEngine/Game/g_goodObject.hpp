#ifndef INCUHENGINE_G_GOODOBJECT_HPP
#define INCUHENGINE_G_GOODOBJECT_HPP
#include <Game/GameObject.hpp>
#include <Bindings.hpp>
#include <SDL2/SDL_events.h>
#include <Model.hpp>
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
                model->getPhysicsMesh()->applyForce(direction, 10.0f);

                Model *goodBox = (Model*) mainState->mainWorld->getObjectByName("box1");
                assert(goodBox == NULL);
                direction[0] = mainState->mainCamera->getPos().x - goodBox->getPosition().x;
                direction[1] = 0.0f;
                direction[2] = mainState->mainCamera->getPos().z - goodBox->getPosition().z;
                goodBox->getPhysicsMesh()->applyForce(direction, 1.0f);

            //}
        }

        void onTriggerEnter(Model *model){
            printf("Found the funny trigger\n");
            if (model == NULL){
                printf("Sadness\n");
            }
            else if (model->getPhysicsMesh() == NULL){
                printf("Some Sadness\n");
            }
            else {
                printf("Got the funny model; %s\n", model->getPhysicsMesh()->getName());
            }
        }
    private:
    };
}

#endif //INCUHENGINE_G_GOODOBJECT_HPP
