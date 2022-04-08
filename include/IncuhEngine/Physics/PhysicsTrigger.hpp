#ifndef PHYSICSTRIGGER_HPP
#define PHYSICSTRIGGER_HPP
#include <Physics/PhysicsObject.hpp>
#include <Physics/PhysicsMaterial.hpp>
#include <Physics/PhysicsCore.hpp>
#include <stdScale.h>

class PhysicsTrigger: public PhysicsObject{
    public:
        PhysicsTrigger(const char *name, physx::PxVec3 position, float scale, PhysicsMaterial *dMat, PhysicsCore *core, triggerCallbacks *callbackValues);

        virtual ~PhysicsTrigger();
    private:


};

#endif // PHYSICSTRIGGER_HPP
