#ifndef PHYSICSTRIGGERCALLBACK_HPP
#define PHYSICSTRIGGERCALLBACK_HPP
#include <physx/PxPhysicsAPI.h>
#include <physx/PxSimulationEventCallback.h>
#include <stdio.h>
#include <stdScale.h>
#include <Server/Logging.h>

using namespace physx;

class PhysicsTriggerCallback : public PxSimulationEventCallback
{
    public:
        PhysicsTriggerCallback();
        virtual ~PhysicsTriggerCallback();

        //virtual void							onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {}
        virtual void							onContact(const PxContactPairHeader&, const PxContactPair*, PxU32) {}
        virtual void							onTrigger(PxTriggerPair* pairs, PxU32 count);
        virtual void							onConstraintBreak(PxConstraintInfo*, PxU32) {}
        virtual void							onWake(PxActor** , PxU32 ) {}
        virtual void							onSleep(PxActor** , PxU32 ){}
        virtual void							onAdvance(const PxRigidBody*const*, const PxTransform*, const PxU32) {}

    protected:

    private:
};

#endif // PHYSICSTRIGGERCALLBACK_HPP
