#include <Physics/PhysicsCapsuleController.hpp>

PhysicsCapsuleController::PhysicsCapsuleController(PhysicsMaterial *dMat, float height, float radius, float stepOffset, float scaleCoeff, float volumeGrowth, float density, float slopeLimit){
    pMainController = NULL;
    pChrDec = new physx::PxCapsuleControllerDesc();
    pChrDec->height = height;
    pChrDec->radius = radius;
    pChrDec->stepOffset = stepOffset;
    pChrDec->material = pMainMat;
    pChrDec->scaleCoeff = scaleCoeff;
    pChrDec->volumeGrowth = volumeGrowth;
    pChrDec->density = density;
    pChrDec->slopeLimit = slopeLimit;
    pChrDec->material = dMat->getMaterial();
    pChrDec->climbingMode = physx::PxCapsuleClimbingMode::eEASY;
    pChrDec->maxJumpHeight = 2.0f;
}

PhysicsCapsuleController::~PhysicsCapsuleController(){
    if (pMainController){
        pMainController->release();
    }
    delete pChrDec;
}

void PhysicsCapsuleController::makeConfig(physx::PxControllerManager *controllerManager){
    pMainController = controllerManager->createController(*pChrDec);
}
