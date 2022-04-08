#include <Physics/PhysicsTrigger.hpp>

PhysicsTrigger::PhysicsTrigger(const char *name, physx::PxVec3 position, float scale, PhysicsMaterial *dMat, PhysicsCore *core, triggerCallbacks *callbackValues){
    pMainMat = dMat->getMaterial();
    pMainRigid = (physx::PxRigidBody*) core->getPhysics()->createRigidStatic(physx::PxTransform(position));
    physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,0,1)));

    pMainShape = physx::PxRigidActorExt::createExclusiveShape(*pMainRigid, physx::PxBoxGeometry(scale, scale, scale), *pMainMat);
    pMainShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    pMainShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

    pMainShape->setLocalPose(relativePose);
    pMainRigid->setName(name);
    pMainRigid->userData = malloc(sizeof(triggerCallbacks));
    memcpy(pMainRigid->userData, callbackValues, sizeof(triggerCallbacks));
}

PhysicsTrigger::~PhysicsTrigger(){

}
