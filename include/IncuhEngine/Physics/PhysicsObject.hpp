#ifndef PHYSICS_OBJECT
#define PHYSICS_OBJECT
#include <physx/PxPhysicsAPI.h>

// A abstract class for physics objects

enum RigidTypes{
    STATIC,
    DYNAMIC
};

class PhysicsObject{
    public:

        void getPosition(float *target) {
            if (target == NULL || pMainRigid == NULL){
                return;
            }
            target[0] = pMainRigid->getGlobalPose().p.x;
            target[1] = pMainRigid->getGlobalPose().p.y;
            target[2] = pMainRigid->getGlobalPose().p.z;
        }

        void getRotation(float *target){
        	if (target == NULL || pMainRigid == NULL){
        		return;
        	}
        	target[0] = pMainRigid->getGlobalPose().q.w;
        	target[1] = pMainRigid->getGlobalPose().q.x;
        	target[2] = pMainRigid->getGlobalPose().q.y;
        	target[3] = pMainRigid->getGlobalPose().q.z;
        }
        void setPosition(float *target) {
        	if (target == NULL || pMainRigid == NULL){
        		return;
            }
            pMainRigid->setGlobalPose(physx::PxTransform(physx::PxVec3( target[0], target[1], target[2] )));
       }



        physx::PxRigidBody *getRigidBody(){ return pMainRigid; }
        physx::PxMaterial *getMaterial() { return pMainMat; }
        physx::PxShape *getShape() { return pMainShape; }
        void setName(const char *newName) { pMainRigid->setName(newName); name = newName; }
        const char* getName() { return name; }
    protected:
        physx::PxRigidBody *pMainRigid;
        physx::PxMaterial *pMainMat;
        physx::PxShape *pMainShape;
        const char *name;
};

#endif
