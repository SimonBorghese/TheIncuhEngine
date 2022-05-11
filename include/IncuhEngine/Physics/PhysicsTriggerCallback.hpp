#ifndef PHYSICSTRIGGERCALLBACK_HPP
#define PHYSICSTRIGGERCALLBACK_HPP
#include <GameState.h>
#include <Launcher/LauncherArgs.hpp>
#include <Window.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Bindings.hpp>
#include <glm/glm.hpp>
#include <AudioBase.hpp>
#include <physx/PxPhysicsAPI.h>
#include <physx/PxSimulationEventCallback.h>
#include <Physics/PhysicsCore.hpp>
#include <stdio.h>
#include <stdScale.h>
#include <Server/Logging.h>
#include <Engine/AudioBase.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <functional>
#include <Game/GameObject.hpp>
#include <Game/GameMain.hpp>

using namespace physx;

typedef struct{
    void (*levelChange)(std::string);
} CallbackFunctions;

class PhysicsTriggerCallback : public PxSimulationEventCallback
{
    public:
        PhysicsTriggerCallback(IncuhState *state, CallbackFunctions callbacks);
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
        CallbackFunctions pFunctions;
        triggerCallbacks *pCallback;
        IncuhState *tState;
};

#endif // PHYSICSTRIGGERCALLBACK_HPP
