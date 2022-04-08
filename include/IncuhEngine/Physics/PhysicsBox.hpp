#ifndef PHYSICS_BOX
#define PHYSICS_BOX
#include <Physics/PhysicsObject.hpp>
#include <Physics/PhysicsMaterial.hpp>
#include <Physics/PhysicsCore.hpp>

class PhysicsBox: public PhysicsObject{
    public:
        PhysicsBox(physx::PxVec3 position, physx::PxVec3 scale, RigidTypes rigidType, PhysicsMaterial *dMat, PhysicsCore *core);

        virtual ~PhysicsBox();
    private:


};

#endif