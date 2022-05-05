#ifndef INCUHENGINE_G_GOODOBJECT_HPP
#define INCUHENGINE_G_GOODOBJECT_HPP
#include <Game/GameObject.hpp>
#include <Model.hpp>
namespace Incuh {
    class g_goodObject : public GameObject {
    public:
        g_goodObject() {}

        void start(){}
        void update() {
            for (unsigned int g = 0; g < models.size(); g++){
                float direction[3];
                direction[0] = 0.0f;
                direction[1] = 0.0f;
                direction[2] = 1.0f;
                models.at(g)->getPhysicsMesh()->applyForce(direction, 20.0f);
            }
        }
    private:
    };
}

#endif //INCUHENGINE_G_GOODOBJECT_HPP
