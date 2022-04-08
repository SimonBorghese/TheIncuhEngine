#ifndef N_BASENPC_HPP
#define N_BASENPC_HPP
#include <glm/glm.hpp>
/*
NPC Concept:
-Class manages position & rotation (Maybe make Physics based NPC a child class?)
-Model can be bound to a NPC & will copy position
-Maybe use move target instead of setting position
-Move torwards something at speed
-Get next move target to move
*/


class n_baseNPC
{
    public:
        n_baseNPC(glm::vec3 pos);
        virtual ~n_baseNPC();

        void setMoveTarget(glm::vec3 target, float speed);
        glm::vec3 getNextMovementTarget();

        glm::vec3 getPos();
        float getSpeed();


    protected:
        glm::vec3 nPos;
        float nSpeed = 0.0f;

        glm::vec3 nTarget = glm::vec3(0.0f);
        glm::vec3 nMoveTarget = glm::vec3(0.0f);
        float currentOffset = 1;

    private:
};

#endif // N_BASENPC_HPP
