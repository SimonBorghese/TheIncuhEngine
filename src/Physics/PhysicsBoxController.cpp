#include <Physics/PhysicsBoxController.hpp>

PhysicsBoxController::PhysicsBoxController(PhysicsMaterial *dMat, float height, float side, float forward, float stepOffset, float scaleCoeff, float volumeGrowth, float density, float slopeLimit){
    pChrDec = new physx::PxBoxControllerDesc();
    pChrDec->halfHeight = height;
    pChrDec->halfSideExtent = side;
    pChrDec->halfForwardExtent = forward;
    pChrDec->stepOffset = stepOffset;
    pChrDec->material = pMainMat;
    pChrDec->scaleCoeff = scaleCoeff;
    pChrDec->volumeGrowth = volumeGrowth;
    pChrDec->density = density;
    pChrDec->slopeLimit = slopeLimit;
    pChrDec->material = dMat->getMaterial();
    pChrDec->maxJumpHeight = 2.0f;
}

PhysicsBoxController::~PhysicsBoxController(){
    if (pMainController){
        pMainController->release();
    }
    delete pChrDec;
}

void PhysicsBoxController::makeConfig(physx::PxControllerManager *controllerManager){
    pMainController = controllerManager->createController(*pChrDec);
}
