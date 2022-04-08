#ifndef PHYSICS_CORE
#define PHYSICS_CORE
#include <physx/PxPhysicsAPI.h>
#include <physx/extensions/PxTriangleMeshExt.h>
#include <physx/extensions/PxDefaultErrorCallback.h>
#include <physx/extensions/PxDefaultSimulationFilterShader.h>
#include <physx/characterkinematic/PxController.h>
#include <Physics/PhysicsObject.hpp>
#include <Physics/PhysicsController.hpp>
#include <Physics/PhysicsTriggerCallback.hpp>

class PhysicsCore{
    public:
        PhysicsCore(physx::PxVec3 gravity);
        virtual ~PhysicsCore();

        void addPhysicsObject(PhysicsObject *object);
        void addPhysicsController(PhysicsController *controller, float *startPos);

        void simulate(float step);

        physx::PxPhysics* getPhysics() { return pPhysics; }
        physx::PxCooking* getCooking() { return pCooking; }

    private:
        // Keep clean up private, don't want to accidentially call it and fuck up everything
        void cleanUp();
        physx::PxDefaultErrorCallback pError;
        physx::PxDefaultAllocator pAllocate;

        physx::PxPhysics *pPhysics;
        physx::PxFoundation *pFoundation;
        physx::PxPvd *pPvd;
        physx::PxPvdTransport* pTransport;

        physx::PxCooking *pCooking;

        physx::PxDefaultCpuDispatcher *pCpuDispatcher;

        physx::PxScene *pScene;
        physx::PxSceneDesc *pPhysDec;

        physx::PxControllerManager *pControlManager;
};
#endif
