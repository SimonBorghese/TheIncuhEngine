#ifndef GAMESTATE_H
#define GAMESTATE_H
#include <Launcher/LauncherArgs.hpp>
#include <Window.hpp>
#include <World.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Bindings.hpp>
#include <WorldLoader.hpp>
#include <glm/glm.hpp>
#include <AudioBase.hpp>
#include <CubeMap.hpp>
#include <Physics/PhysicsCore.hpp>
#include <Physics/PhysicsBox.hpp>
#include <Physics/PhysicsCapsuleController.hpp>
#include <Physics/PhysicsMesh.hpp>
#include <Physics/PhysicsBoxController.hpp>

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


#endif // GAMESTATE_H
