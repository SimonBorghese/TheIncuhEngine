#ifndef GAMESTATE_H
#define GAMESTATE_H

namespace LArgs{
    class LauncherArgs;
}
class Window;
class Shader;
class World;
class Bindings;
class Camera;
class CubeMap;
class WorldLoader;
class AudioBase;
class PhysicsCore;
class PhysicsBoxController;
class PhysicsCapsuleController;
class PhysicsMaterial;
#include <glm/glm.hpp>


// Long constructors solved
typedef struct{
    glm::mat4 *model, *view, *projection, *ortho;

    LArgs::LauncherArgs *mainArgs;
    Window              *mainWindow;
    Shader              *mainShader, *skyBox;
    World               *mainWorld;
    Bindings            *mainBindings;
    Camera              *mainCamera;
    CubeMap             *mainSkybox;
    WorldLoader         *mainLoader;
    AudioBase           *mainAudio;
    PhysicsCore *mainPhysics;
    PhysicsBoxController *boxController;
    PhysicsCapsuleController *mainController;
    PhysicsMaterial *basicMaterial;
} IncuhState;

// Do not enable if using with a class that exists within the state



#endif // GAMESTATE_H
