#include <Physics/PhysicsTrigger.hpp>

PhysicsTrigger::PhysicsTrigger(const char *name, physx::PxVec3 position, float scale, PhysicsMaterial *dMat, PhysicsCore *core, triggerCallbacks *callbackValues){
    pMaterial = dMat;
    pMainRigid = (physx::PxRigidBody*) core->getPhysics()->createRigidStatic(physx::PxTransform(position));
    physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,0,1)));

    pMainShape = physx::PxRigidActorExt::createExclusiveShape(*pMainRigid, physx::PxBoxGeometry(scale, scale, scale), *pMaterial->getMaterial());
    pMainShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
    pMainShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

    pMainShape->setLocalPose(relativePose);
    pMainRigid->setName(std::string(name).c_str());
    pMainRigid->userData = malloc(sizeof(triggerCallbacks));
    //memcpy(pMainRigid->userData, callbackValues, sizeof(triggerCallbacks));
    *((triggerCallbacks*)pMainRigid->userData) = *callbackValues;
}

PhysicsTrigger::~PhysicsTrigger(){
    delete pMaterial;
    free(pMainRigid->userData);
    pMainRigid->release();
    pMainShape->release();

}
