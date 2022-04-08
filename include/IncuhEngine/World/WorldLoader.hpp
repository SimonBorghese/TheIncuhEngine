#ifndef WORLDLOADER_H
#define WORLDLOADER_H
#include <World.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Server/Logging.h>
#include <CBSP/CBSP.h>
#include <BSP/BSPMesh.hpp>
#include <fstream>
#include <AudioBase.hpp>
#include <map>
#include <functional>
#ifndef NOEXERNLIBS
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fx/gltf.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <Physics/PhysicsCore.hpp>
#include <Physics/PhysicsTrigger.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
//#include <dlfcn.h>
#include <stdScale.h>
#endif

/*
World Loader:
    Loads vertices into World and Creates World Object
*/

#define BSPSCALE (1.0f/16.0f)

/*
WorldLoader::WorldLoader(const char *fileName, Shader *shader, World *__target, PhysicsCore *core, Camera *cam) : __world(__target), __shader(shader), __core(core), __fileName(fileName){
*/

enum ENTITES_TYPES{
    PROP,
    LIGHT,
    SUN,
    TRIGGER
};

class WorldLoader: public Object
{
    public:
        WorldLoader(const char *fileName, Shader *shader, World *__target, PhysicsCore *core, Camera *cam);
        virtual ~WorldLoader();

        void update();

    protected:

    private:
        const char *__fileName;
        std::map<std::string, ENTITES_TYPES> entityMap;
        CBSP *bsp = nullptr;
};

#endif // WORLDLOADER_H
