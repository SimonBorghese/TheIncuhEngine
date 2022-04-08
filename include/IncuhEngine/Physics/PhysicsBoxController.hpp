#ifndef BOX_CONTROLLER
#define BOX_CONTROLLER
#include <Physics/PhysicsController.hpp>
#include <Physics/PhysicsMaterial.hpp>
#include <Physics/PhysicsCore.hpp>

class PhysicsBoxController: public PhysicsController{
    public:
		PhysicsBoxController(PhysicsMaterial *dMat, float height, float side, float forward, float stepOffset, float scaleCoeff, float volumeGrowth, float density, float slopeLimit);

        virtual ~PhysicsBoxController();

        void makeConfig(physx::PxControllerManager *controllerManager);
    private:
        physx::PxBoxControllerDesc *pChrDec = nullptr;

};

#endif
