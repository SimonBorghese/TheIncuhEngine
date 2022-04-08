#include <NPC/n_baseNPC.hpp>

n_baseNPC::n_baseNPC(glm::vec3 pos) : nPos(pos)
{}

n_baseNPC::~n_baseNPC()
{
    //dtor
}

void n_baseNPC::setMoveTarget(glm::vec3 target, float speed){
    nTarget = target;
    nSpeed = speed;
    currentOffset = 1;
    nMoveTarget = glm::vec3( ( nTarget / speed) * currentOffset);

}
glm::vec3 n_baseNPC::getNextMovementTarget(){
    nMoveTarget = glm::vec3( ( nTarget / nSpeed));
    return nMoveTarget;
}

glm::vec3 n_baseNPC::getPos() { return nPos; }
float n_baseNPC::getSpeed() {return nSpeed;}
