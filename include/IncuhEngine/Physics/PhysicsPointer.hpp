#ifndef PHYSICSPOINTER_HPP
#define PHYSICSPOINTER_HPP
#include <Physics/PhysicsObject.hpp>
#include <string>
#include <vector>

class PhysicsPointer{
    public:
        PhysicsPointer(physx::PxRigidActor *body, physx::PxShape *shape, std::string name){
            pMainRigid = body; pMainShape = shape; this->name=name.c_str();
        }

        physx::PxRigidActor *pMainRigid;
        physx::PxShape *pMainShape;
        const char *name;


};

#endif // PHYSICSPOINTER_HPP
