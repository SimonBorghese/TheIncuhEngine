#ifndef WORLDLOADER_H
#define WORLDLOADER_H
#include <GameState.h>
#include <World.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Launcher/LauncherArgs.hpp>
#include <Server/Logging.h>
#include <CBSP/CBSP.h>
#include <BSP/BSPMesh.hpp>
#include <fstream>
#include <AudioBase.hpp>
#include <map>
#include <vector>
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
        //WorldLoader(const char *fileName, Shader *shader, World *__target, PhysicsCore *core, Camera *cam);
        WorldLoader(IncuhState *state);
        virtual ~WorldLoader();

        void update();

        const char * CB(const char *inputStr){
            if (strcmp(inputStr, CBSP_getKeyFromEntity_FAILURE) && strcmp(inputStr, CBSP_EOF_FAILURE)){
                __stringHolder.push_back( (char*) inputStr);
            }
            return inputStr;
        }

        void* GB(void *inputitem){
            if (inputitem != NULL){
                __itemHolder.push_back(inputitem);
            }
            return inputitem;

        }

        void KillCB(){
            for (int c = __stringHolder.size() - 1; c >= 0; c--){
                free ( (char*) __stringHolder.at(c));
            }
            for (int c = __itemHolder.size() - 1; c >= 0; c--){
                delete ( (char*) __itemHolder.at(c));
            }
        }


    protected:

    private:
        //const char *__fileName;
        std::map<std::string, ENTITES_TYPES> entityMap;
        CBSP *bsp = nullptr;
        std::vector<void*> __itemHolder;
        std::vector<char*> __stringHolder;
};

#endif // WORLDLOADER_H
