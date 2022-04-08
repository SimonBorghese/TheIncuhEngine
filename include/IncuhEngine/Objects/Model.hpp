#ifndef MODEL_HPP
#define MODEL_HPP
#include <vector>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <math.h>
#include <Shader.hpp>
#include <Image.hpp>
#include <Object.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <Server/Logging.h>
#ifndef NOEXERNLIBS
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <nlohmann/json.hpp>
#include <fstream>
#endif
#include <Physics/PhysicsMesh.hpp>
#include <Physics/PhysicsCore.hpp>
#include <Physics/PhysicsBox.hpp>
#include <Physics/PhysicsController.hpp>
#include <NPC/n_baseNPC.hpp>



/*
 The Model Class, REVISED EDITION:

 To be obsolete:
    data/textures/ *
    data/{any 3d model file}
    EXCEPTIONS:
        data/cube.FBX
        missing.png

 New Method:
    The structure of the data folder should now look like this:
        data/[model name]/model.*
        data/{model name}/textures/{Will contain all the original textures}
        data/{model name}/meta.json (REQUIRED)
    Now, instead of loading by full file name, models should now just be loaded by their name
    No need for texture folder hell

    meta.json:
        Contain essential data about the model
        The following values MUST be set:
            Model Name : Contains the name of the 3d model file
            Position : Default position
                AXIS {x,y,z} : x,y, or z value
            Rotation : Default Quaternion
                AXIS {w,x,y,z} : Quaternion
            Scale : Default Scale
                AXIS {x,y,z} : x,y, or z value

            HasCollision : Defines if the object has a collider

            collider :
                AXIS {x,y,z} : The extends of the box collider


*/

#define DEFAULTMODEL "data/cube.FBX"


class Model: public Object
{
public:
    // For direct model loading
    ////Model( glm::vec3 pos, glm::vec3 rot, glm::vec3 scle, const char *fileName, Shader *shader, Image *diffuse, float mass, Material *mat, float rotation = 270.0f, int useMeshCollider = 0, PhysicsCore *core = NULL);
    Model( const char *name, glm::vec3 pos, glm::quat quat, glm::vec3 scle, const char *fileName, Shader *shader, Image *diffuse, Material *mat, int useMeshCollider, PhysicsCore *core, int dynamic = 1);

    Model();

    virtual ~Model();

    void render();

    void update();

    void rotate(float degres, glm::vec3 axis);

    void bindController(PhysicsController *targetController);

    void translate(float *newPos);

    PhysicsObject* getPhysicsMesh() { return oMesh; }

    void bindNPC(n_baseNPC *npc);
protected:
    // The Asssimp object for importing
    Assimp::Importer *oModelLoader;

    // The scene for our object
    const aiScene *oScene;

    Image *oDiffuse;
    Shader *oShader;

    glm::quat oQuat;
	glm::vec3 oScale;
    glm::mat4 *oModel;
    uint32_t oModelPos;
    Material *oMat;

    std::vector<Mesh*> oMeshes;

    const char *oModelName;

    int oHasCollider;

    PhysicsObject *oMesh;

    float* oTargetHolder;

    float oMass = 0.0f;
    float oSMI = 0.0f;
    float oDegree = 0.0f;

    PhysicsController *oBindControl = nullptr;

    int oUseMeshCollider;
    n_baseNPC *oNpc = NULL;
};
#endif
