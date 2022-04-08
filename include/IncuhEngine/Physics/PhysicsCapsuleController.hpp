#ifndef CAPSULE_CONTROLLER
#define CAPSULE_CONTROLLER
#include <Physics/PhysicsController.hpp>
#include <Physics/PhysicsMaterial.hpp>
#include <Physics/PhysicsCore.hpp>

class PhysicsCapsuleController: public PhysicsController{
    public:
        PhysicsCapsuleController(PhysicsMaterial *dMat, float height, float radius, float stepOffset, float scaleCoeff, float volumeGrowth, float density, float slopeLimit);

        virtual ~PhysicsCapsuleController();

        void makeConfig(physx::PxControllerManager *controllerManager);
    private:
        physx::PxCapsuleControllerDesc *pChrDec;

};

#endif
