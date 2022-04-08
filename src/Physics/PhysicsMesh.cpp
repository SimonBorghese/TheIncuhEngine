#include "PxShape.h"
#include "cooking/PxConvexMeshDesc.h"
#include "cooking/PxTriangleMeshDesc.h"
#include "foundation/PxFlags.h"
#include "geometry/PxSimpleTriangleMesh.h"
#include "geometry/PxTriangleMesh.h"
#include "geometry/PxTriangleMeshGeometry.h"
#include <Physics/PhysicsMesh.hpp>
#include <cstdint>

PhysicsMesh::PhysicsMesh(physx::PxVec3 position, std::vector<float> *vertices, RigidTypes rigidType, PhysicsMaterial *dMat, PhysicsCore *core){
    pMainMat = dMat->getMaterial();
    if (rigidType == DYNAMIC){
        pMainRigid = core->getPhysics()->createRigidDynamic(physx::PxTransform(position));
        physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,0,1)));

        // BEGIN MESH GEN
            physx::PxConvexMeshDesc convexDesc;
            convexDesc.points.count     = (vertices->size()/3);
            convexDesc.points.stride    = sizeof(physx::PxVec3);
            convexDesc.points.data      = &(vertices->data()[0]);
            convexDesc.flags            = physx::PxConvexFlag::eCOMPUTE_CONVEX;

            physx::PxConvexMesh* convexMesh = core->getCooking()->createConvexMesh(convexDesc, core->getPhysics()->getPhysicsInsertionCallback());

            pMainShape = physx::PxRigidActorExt::createExclusiveShape(*pMainRigid,
                physx::PxConvexMeshGeometry(convexMesh), *pMainMat);

        // END MESH GEN

        pMainShape->setLocalPose(relativePose);

        pMainRigid->setMass(dMat->getMass());
        pMainRigid->setMassSpaceInertiaTensor(physx::PxVec3(dMat->getSpaceinteriamass()));
        physx::PxRigidBodyExt::updateMassAndInertia(*pMainRigid, 5.0f);
        //scene->addActor(*dMainRigid);
    }else{

        pMainRigid = (physx::PxRigidBody*) core->getPhysics()->createRigidStatic(physx::PxTransform(position));
        physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f,0.0f,0.0f)));

        // BEGIN MESH GEN
            physx::PxConvexMeshDesc convexDesc;
            convexDesc.points.count     = (vertices->size()/3);
            convexDesc.points.stride    = sizeof(physx::PxVec3);
            convexDesc.points.data      = &(vertices->data()[0]);
            convexDesc.flags            = physx::PxConvexFlag::eCOMPUTE_CONVEX;

            physx::PxDefaultMemoryOutputStream buf;
            physx::PxConvexMeshCookingResult::Enum result;
            if(!core->getCooking()->cookConvexMesh(convexDesc, buf, &result)){
                printf("FAILED TO MAKE CONVEX MESH: %d\n", result);
            }
            physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
            physx::PxConvexMesh* convexMesh = core->getPhysics()->createConvexMesh(input);


            //physx::PxConvexMesh* convexMesh = core->getCooking()->createConvexMesh(convexDesc, core->getPhysics()->getPhysicsInsertionCallback());

            pMainShape = physx::PxRigidActorExt::createExclusiveShape(*pMainRigid,
                physx::PxConvexMeshGeometry(convexMesh), *pMainMat);

        // END MESH GEN
        if (pMainShape != NULL){
            pMainShape->setLocalPose(relativePose);
        }
        else{
            pMainRigid = NULL;
        }
        //pMainRigid->setGlobalPose(physx::PxTransform(position, physx::PxQuat(physx::PxPi, physx::PxVec3(0.0f, 1.0f, 0.0f))));

    }
    //dMainRigid->setMass(5.0f);
    //dMainRigid->setMassSpaceInertiaTensor(physx::PxVec3(5.0f));
}
PhysicsMesh::PhysicsMesh(physx::PxVec3 position, std::vector<float> *vertices, std::vector<uint32_t> *indicies, RigidTypes rigidType, PhysicsMaterial *dMat, PhysicsCore *core){
    pMainMat = dMat->getMaterial();
    if (rigidType == DYNAMIC){
        pMainRigid = core->getPhysics()->createRigidDynamic(physx::PxTransform(position));
        physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,0,1)));

        // BEGIN MESH GEN
            physx::PxTriangleMeshDesc convexDesc;
            convexDesc.points.count     = (vertices->size()/3);
            convexDesc.points.stride    = sizeof(physx::PxVec3);
            convexDesc.points.data      = &(vertices->data()[0]);
            ////convexDesc.flags            = physx::PxConvexFlags::eFLIPNORMALS;


            convexDesc.triangles.count = (indicies->size()/3);
            convexDesc.triangles.data = &(indicies->data()[0]);
            convexDesc.triangles.stride = 3 * sizeof(uint32_t);

            physx::PxTriangleMesh* convexMesh = core->getCooking()->createTriangleMesh(convexDesc, core->getPhysics()->getPhysicsInsertionCallback());

            pMainShape = physx::PxRigidActorExt::createExclusiveShape(*pMainRigid,
                physx::PxTriangleMeshGeometry(convexMesh), *pMainMat);

        // END MESH GEN

        pMainShape->setLocalPose(relativePose);
        physx::PxRigidBodyExt::updateMassAndInertia(*pMainRigid, 1.0f);
        //scene->addActor(*dMainRigid);
    }else{

        pMainRigid = (physx::PxRigidBody*) core->getPhysics()->createRigidStatic(physx::PxTransform(position));
        physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0,0,1)));

        // BEGIN MESH GEN
            physx::PxTriangleMeshDesc convexDesc;
            convexDesc.points.count     = (vertices->size()/3);
            convexDesc.points.stride    = sizeof(physx::PxVec3);
            convexDesc.points.data      = &(vertices->data()[0]);
            //convexDesc.flags            = physx::PxConvexFlag::eCOMPUTE_CONVEX;

            convexDesc.triangles.count = (indicies->size()/3);
            convexDesc.triangles.data = &(indicies->data()[0]);
            convexDesc.triangles.stride = 3 * sizeof(uint32_t);

            physx::PxTriangleMesh* convexMesh = core->getCooking()->createTriangleMesh(convexDesc, core->getPhysics()->getPhysicsInsertionCallback());

            pMainShape = physx::PxRigidActorExt::createExclusiveShape(*pMainRigid,
                physx::PxTriangleMeshGeometry(convexMesh), *pMainMat);

        // END MESH GEN

        pMainShape->setLocalPose(relativePose);
    }
}

PhysicsMesh::~PhysicsMesh(){

}
