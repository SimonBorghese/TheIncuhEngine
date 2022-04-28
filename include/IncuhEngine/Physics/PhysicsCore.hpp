#ifndef PHYSICS_CORE
#define PHYSICS_CORE
#include <glm/glm.hpp>
#include <string>
#include <physx/PxPhysicsAPI.h>
#include <physx/extensions/PxTriangleMeshExt.h>
#include <physx/extensions/PxDefaultErrorCallback.h>
#include <physx/extensions/PxDefaultSimulationFilterShader.h>
#include <physx/characterkinematic/PxController.h>
#include <Physics/PhysicsObject.hpp>
#include <Physics/PhysicsPointer.hpp>
#include <Physics/PhysicsController.hpp>
#include <Physics/PhysicsTriggerCallback.hpp>

class PhysicsCore{
    public:
        PhysicsCore(physx::PxVec3 gravity);
        virtual ~PhysicsCore();

        void addPhysicsObject(PhysicsObject *object);
        void addPhysicsController(PhysicsController *controller, float *startPos);

        void simulate(float step);

        void softCleanup();

        physx::PxPhysics* getPhysics() { return pPhysics; }
        physx::PxCooking* getCooking() { return pCooking; }

        void setTriggerCallback(PhysicsTriggerCallback *callback) {  pScene->setSimulationEventCallback(callback); }

        PhysicsPointer* rayCast(float *origin, float *direction, float length){
            PxVec3 porigin = physx::PxVec3(origin[0], origin[1], origin[2]);
            PxVec3 punitDir = physx::PxVec3(direction[0], direction[1], direction[2]);
            PxReal pmaxDistance = length;
            PxRaycastBuffer hit;

            PxQueryFilterData filterData(PxQueryFlag::eDYNAMIC);
            bool status = pScene->raycast(porigin, punitDir, pmaxDistance, hit, physx::PxHitFlag::eDEFAULT, filterData);
            if (status){
                if (hit.block.shape->getActor()->userData != NULL){

                    printf("HIT SOMETHING: %s\n", ((std::string*) hit.block.shape->getActor()->userData)->c_str());

                }
                printf("Looking at: %f %f %f, staring at: %f %f %f\n", hit.block.shape->getActor()->getGlobalPose().p.x,hit.block.shape->getActor()->getGlobalPose().p.y,hit.block.shape->getActor()->getGlobalPose().p.z, porigin.x,porigin.y,porigin.z);
                return (new PhysicsPointer(hit.block.actor, hit.block.shape, std::string(hit.block.shape->getActor()->getName())) );
            }
            return NULL;
        }

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
