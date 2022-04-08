#ifndef WORLD_H
#define WORLD_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <Model.hpp>
#include <glm/glm.hpp>
#include <Shader.hpp>
#include <Object.hpp>
#include <Model.hpp>
#include <Light.hpp>
#include <Image.hpp>
#include <Material.hpp>
#include <Camera.hpp>
#include <Window.hpp>
#include <Server/Logging.h>
#include <fmt/format.h>
#ifndef NOEXERNLIBS
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <Physics/PhysicsCore.hpp>
#include <Physics/PhysicsController.hpp>
#endif



/*
 The World V3:

 World loads the world from a map file located in maps/

 World should load all world meshes with the model class (create constructor override that takes aiMesh?) and then all collidiers with a collision class?
 Entities should be loaded with an entity class
 info_player_start?

*/

class World
{
public:
    World(Shader *shader, glm::mat4 *model, glm::mat4 *view, glm::mat4 *projection);
    virtual ~World();

    glm::vec3 getGravity();

    void cacheLocation();
    void updateMat();
    void useMain();

    Shader *getShader();

    uint32_t getUniformLoc(const char *name);
    void setVec3(uint32_t loc, glm::vec3 vector);

    void addObject(Object *obj, const char *name);

    Model* createObject( const char *name, glm::vec3 pos, glm::quat rot, glm::vec3 scle, const char *fileName, Image *diffuse, Material *mat = NULL, int useCollider = 0, PhysicsCore *core = NULL, int dynamic = 1);

    Light* createPointLight( const char *name, glm::vec3 pos, glm::vec3 *ambient, glm::vec3 *diffuse, glm::vec3 *specular, float const_val, float linear_val, float quadratic_val);

    Light* createDirectionalLight( const char *name, glm::vec3 direction, glm::vec3 *ambient, glm::vec3 *diffuse, glm::vec3 *specular);

    Camera* createCamera( const char *name, glm::vec3 newCameraPos, float speed, glm::mat4 *view_mat, Window *win, PhysicsController *control, PhysicsCore *core);

    void updateObjects();

    Object* getObjectByName(const char *name);

private:
    // Our shader object
    Shader *__shader;

    // Our model, view, and projection matrices
    glm::mat4 *__model, *__view, *__projection;

    // The Uniform locations of these matrices
    uint32_t __model_loc, __view_loc, __projection_loc;

    // A vector of all our objects
    std::vector<Object*> __world_objects;

    uint32_t __lightCount;
    uint32_t __lightCount_loc;
    #ifndef NOEXERNLIBS
    const aiScene *mMainWorld;
    Assimp::Importer *model_loader;
    #endif

    std::map<std::string, Object*> mWorldObjects;


};


#endif
