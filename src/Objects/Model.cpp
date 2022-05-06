#include "Physics/PhysicsObject.hpp"
#include <Model.hpp>
#include <assimp/postprocess.h>
#include <vector>

/*
* New Texture loading concept:
* Maybe make a mesh class with their own VBOs and texture data; use Model as a master class to load in these meshes,  programmer doesn't need to touch mesh maybe?
* Loading in many VAO intensive?
* Load in
*/

Model::Model( const char *name, glm::vec3 pos, glm::quat quat, glm::vec3 scle, const char *fileName, Shader *shader, Image *diffuse, Material *mat, int useMeshCollider, PhysicsCore *core, int dynamic) :
oModelName(fileName),
oMass(0.0f),
oSMI(0.0f),
oDegree(0.0f),
oUseMeshCollider(useMeshCollider)
{
    nlohmann::json basefile;
    // First, load the JSON file in the directory

    oTargetHolder = (float*) (malloc(sizeof(float) * 4));
    oTargetHolder[0] = 0.0f;
    oTargetHolder[1] = 0.0f;
    oTargetHolder[2] = 0.0f;
    oTargetHolder[3] = 0.0f;
    if (!useMeshCollider){
        std::ifstream file(((std::string) fmt::format("data/ent/{}/meta.json", fileName)).c_str());
        file >> basefile;
        file.close();
    }

    std::string fileLoc;
    if (!useMeshCollider){
        fileLoc = fmt::format("data/ent/{}/{}", fileName, basefile["model"]);
    }
    else{
        fileLoc = fmt::format("maps/{}/level.obj", fileName);
    }
    oModelLoader = new Assimp::Importer;
    oScene = oModelLoader->ReadFile(fileLoc.c_str(), aiProcess_Triangulate);
    if (strcmp("", oModelLoader->GetErrorString()) != 0)
    {
        incuh_warning(fmt::format("Failed to load: {}, Error: {}, defaulting to: {}\n", fileLoc.c_str(), oModelLoader->GetErrorString(), DEFAULTMODEL));
        oModelLoader = new Assimp::Importer;
        oScene = oModelLoader->ReadFile(DEFAULTMODEL, aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_FindInvalidData | aiProcess_OptimizeMeshes);
        if (strcmp("", oModelLoader->GetErrorString()) != 0)
        {
            incuh_error(fmt::format("FATAL ERROR: FAILED TO LOAD DEFAULT MODEL: {}\n", oModelLoader->GetErrorString()));
        }
    }


    if (diffuse != NULL)
    {
        oDiffuse = diffuse;
    }
    else
    {
        oDiffuse =NULL;
    }

    if (shader != NULL)
    {
        oShader = shader;
    }
    else
    {
        incuh_warning(fmt::format("GOT NO SHADER IN {} OBJECT\n", fileName));
    }


    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    std::string tempStr = "";

    // We must iterate through all our meshes



    // Now we do positional stuff
    __pos = new glm::vec3(pos);
    oQuat = quat;
    //__scale = new glm::vec3(scle);
    oScale = glm::vec3(1.0f);
    oModel = new glm::mat4(1.0f);
    oModelPos = oShader->getUniformLocation("model");

    oMat = mat;
    // Finally, we apply offsets from the file
    // Scale should be multiplied to avoid issues with both specifying 1x

    // This is what I call  p e a k  efficanty
    if (!useMeshCollider){
        __pos->x += (float) basefile["position"]["x"];
        __pos->y += (float) basefile["position"]["y"];
        __pos->z += (float) basefile["position"]["z"];

        oQuat.w += (float) basefile["rotation"]["w"];
        oQuat.x += (float) basefile["rotation"]["x"];
        oQuat.y += (float) basefile["rotation"]["y"];
        oQuat.z += (float) basefile["rotation"]["z"];

        scle.x *= (float) basefile["scale"]["x"];
        scle.y *= (float) basefile["scale"]["y"];
        scle.z *= (float) basefile["scale"]["z"];
        try{
            oHasCollider = (int) basefile["HasCollision"];
            oMass = (float) basefile["mass"];
            oSMI = (float) basefile["SMI"];
        }
        catch (...){
            oHasCollider = 0;
        }

    }
    else{
        oHasCollider = 1;
    }

    incuh_info(fmt::format("Found: {} meshes in {}\n", oScene->mNumMeshes, fileName));
    for (unsigned int m = 0; m < oScene->mNumMeshes; m++)
    {
        tempStr = oScene->mMeshes[m]->mName.C_Str();
        incuh_info(fmt::format("Got: {} Valid: {} {} \n", tempStr.c_str(), strcmp(tempStr.substr(0,8).c_str(), "collider"), strcmp(tempStr.substr(0,3).c_str(), "ent")));
        if (strcmp(tempStr.substr(0,8).c_str(), "collider") != 0){
            if (oDiffuse == NULL)
            {
                oMeshes.push_back(new Mesh(oScene->mMeshes[m], oScene->mMaterials[oScene->mMeshes[m]->mMaterialIndex], oModelName, &scle));
            }
            else
            {
                oMeshes.push_back(new Mesh(oScene->mMeshes[m], NULL, oModelName, &scle));
            }
        }
    }



    if (oHasCollider){
        std::string tempStr;

        std::vector<float> *targetVertices;
        std::vector<uint32_t> *targetIndices;
        //std::vector<PhysicsObject*> targetMeshes;
        for (unsigned int m = 0; m < oScene->mNumMeshes; m++){
            tempStr = oScene->mMeshes[m]->mName.C_Str();
            if (( (strcmp(tempStr.substr(0,8).c_str(), "collider") != 0) || useMeshCollider )){
                targetVertices = new std::vector<float>;
                targetIndices = new std::vector<uint32_t>;
                for (unsigned int x = 0 ; x < oScene->mMeshes[m]->mNumVertices; x++){
                    targetVertices->push_back( oScene->mMeshes[m]->mVertices[x].x * scle.x);
                    targetVertices->push_back( oScene->mMeshes[m]->mVertices[x].y * scle.y);
                    targetVertices->push_back( oScene->mMeshes[m]->mVertices[x].z * scle.z);
                }
                for (unsigned int f = 0; f < oScene->mMeshes[m]->mNumFaces; f++){
                    for (unsigned int i = 0; i < oScene->mMeshes[m]->mFaces[f].mNumIndices; i++){
                        targetIndices->push_back(oScene->mMeshes[m]->mFaces[f].mIndices[i]);
                    }
                }
                float frictionDivider = 1.0f;
                PhysicsMesh *targetMesh;
                if (useMeshCollider || !dynamic){
                    targetMesh = (new PhysicsMesh(physx::PxVec3(__pos->x, __pos->y, __pos->z), targetVertices, STATIC, new PhysicsMaterial((0.2f), (0.4f), (0.0f), core), core ));
                }
                else{
                    targetMesh = (new PhysicsMesh(physx::PxVec3(__pos->x, __pos->y, __pos->z), targetVertices, DYNAMIC, new PhysicsMaterial((0.1f/frictionDivider), (0.2f/frictionDivider), (0.0f), core, oMass, oSMI), core ));
                }
                targetMesh->setName(name);
                core->addPhysicsObject(targetMesh);
                targetMeshes.push_back(targetMesh);
                oMesh = targetMesh;
                delete targetVertices;
                delete targetIndices;
            }
        }
        /*
        for (long unsigned int p = 0; p < targetMeshes.size(); p++){
            oMesh = targetMeshes.at(p);
            oMesh->setName(name);
            core->addPhysicsObject(oMesh);
        }
        if (targetMeshes.size() >= 1){
            oMesh = targetMeshes.at(0);
        }
        */
    }
}
Model::Model(){}
Model::~Model(){
    //printf("Deleting a mesh\n");
    //if (targetMeshes.size() > 2){
        for (long int p = targetMeshes.size()-1; p >= 0; p--){
            //printf("Deleting OWO UWU OWO: %lu\n", p);
            if (targetMeshes.at(p) != NULL){
                delete (PhysicsMesh*) (targetMeshes.at(p));
            }
        }
    //}

    for (int m = oMeshes.size() -1; m >= 0; m--){
        delete (Mesh*) oMeshes.at(m);
    }
    //free(scene);
    delete __pos;
    delete oModelLoader;
    free(oTargetHolder);
    delete oMat;
    delete oModel;

}

void Model::update()
{
    oShader->useMain();
    if (oHasCollider && oBindControl == nullptr){
            //printf("Our Mesh is Null: %d\n", oMesh->getRigidBody() == NULL);
        oMesh->getPosition(oTargetHolder);
        __pos->x = oTargetHolder[0];
        __pos->y = oTargetHolder[1];
        __pos->z = oTargetHolder[2];
        //printf("getting 1 %f %f %f\n", oTargetHolder[0], oTargetHolder[1], oTargetHolder[2]);
        oMesh->getRotation(oTargetHolder);
        oQuat.w = oTargetHolder[0];
		oQuat.x = oTargetHolder[1];
		oQuat.y = oTargetHolder[2];
		oQuat.z = oTargetHolder[3];
    }
    else if (oBindControl != nullptr){
        oBindControl->getPosition(oTargetHolder);
        __pos->x = oTargetHolder[0];
        __pos->y = oTargetHolder[1];
        __pos->z = oTargetHolder[2];
    }
    else{
        __pos->x = 0.0f;
        __pos->y = 0.0f;
        __pos->z = 0.0f;
    }
    /*
    if (oHasCollider){
        oTargetHolder[0] = __pos->x;
        oTargetHolder[1] = __pos->y;
        oTargetHolder[2] = __pos->z;
        oMesh->setPosition(oTargetHolder);
    }
    */


    *oModel = glm::mat4(1.0f);
    *oModel = glm::translate(*oModel, *__pos);
    if (!oUseMeshCollider && oQuat != glm::quat(0.0f, 0.0f, 0.0f, 0.0f)){
        *oModel = glm::rotate(*oModel, glm::angle(oQuat), glm::axis(oQuat));
    }

    *oModel = glm::scale(*oModel, oScale);
    oShader->setMatrix4f(oModelPos, *oModel);
    oMat->apply();
    render();
}


void Model::render()
{
    oShader->useMain();
    for (unsigned int m = 0; m<oMeshes.size(); m++)
    {
        if (oDiffuse == NULL)
        {
            oShader->bindTexture(oMeshes.at(m)->getImage(), "diffuse_mat");
        } else
        {
            oShader->bindTexture(oDiffuse, "diffuse_mat");
        }
        oMeshes.at(m)->update();
    }
}

void Model::bindController(PhysicsController *targetController){
	oBindControl = targetController;
}

void Model::translate(float *newPos){
	if (newPos == NULL) { return;}
	if (oHasCollider && oBindControl == nullptr){
		oMesh->setPosition(newPos);
	}
	else if (oBindControl != nullptr){
	    oBindControl->translate(newPos);
	}

}

void Model::bindNPC(n_baseNPC *npc){
    oNpc = npc;
}
