
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <SDL2/SDL.h>
#include <Launcher/LauncherArgs.hpp>
#include <Window.hpp>
#include <World.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Bindings.hpp>
#include <WorldLoader.hpp>
#include <Game/GameMain.hpp>
#include <glm/glm.hpp>
#include <stdScale.h>
#include <AudioBase.hpp>
#define NDEBUG 1
#include <GameState.h>
#define CBSP_IMPLEMNT 1
#include <BSP/BSPMesh.hpp>
#include <CubeMap.hpp>

#define FPS_MAX 300
#define PLAYER_WALK 0.05f
#define PLAYER_RUN 0.1f
#define MAINCFG "cfg/launch.json"

IE_GameState        gameState;

IncuhState mainState;
GameMain *mainGame;

uint64_t profileTime;


std::vector<Object*> objects;

std::vector<uint8_t> keys;

uint64_t start, start2, end, fps;
int Isrun = 0;


float posHolder[3];

void quit(){
    mainGame->gameClose();
	delete mainState.mainWindow;
	delete mainState.mainShader;

    delete mainState.basicMaterial;
    delete mainState.mainController;
    delete mainState.mainBindings;
    delete mainState.mainLoader;

    delete mainState.mainPhysics;
    exit(0);
}

void run(){
    Isrun = 1;
    mainState.mainCamera->setSpeed(PLAYER_RUN);
}


void setupBinds(){
  mainState.mainBindings->setFunc(SDL_SCANCODE_W, std::bind(&Camera::moveForward, mainState.mainCamera));
  mainState.mainBindings->setFunc(SDL_SCANCODE_S, std::bind(&Camera::moveBackward, mainState.mainCamera));
  mainState.mainBindings->setFunc(SDL_SCANCODE_A, std::bind(&Camera::strafeLeft, mainState.mainCamera));
  mainState.mainBindings->setFunc(SDL_SCANCODE_D, std::bind(&Camera::strafeRight, mainState.mainCamera));

  mainState.mainBindings->setFunc(SDL_SCANCODE_UP, std::bind(&Camera::moveUp, mainState.mainCamera));
  mainState.mainBindings->setFunc(SDL_SCANCODE_DOWN, std::bind(&Camera::moveDown, mainState.mainCamera));
  mainState.mainBindings->setFunc(SDL_SCANCODE_LEFT, std::bind(&Camera::turnLeft, mainState.mainCamera));
  mainState.mainBindings->setFunc(SDL_SCANCODE_RIGHT, std::bind(&Camera::turnRight, mainState.mainCamera));
  mainState.mainBindings->setFunc(SDL_SCANCODE_ESCAPE, quit);
  mainState.mainBindings->setFunc(SDL_SCANCODE_SPACE, std::bind(&Camera::jump, mainState.mainCamera));
  mainState.mainBindings->setFunc(SDL_SCANCODE_LSHIFT, run);
}

uint64_t getNanoseconds(){
    auto time_hp = std::chrono::high_resolution_clock::now();
    return (uint64_t) (std::chrono::duration_cast<std::chrono::nanoseconds>(time_hp.time_since_epoch()).count());
}

uint64_t getSecond(){
    return (((std::chrono::duration_cast<std::chrono::nanoseconds>) (std::chrono::seconds(1))).count());
}


int main(int argc, char **argv) {
    mainState.mainArgs = new LArgs::LauncherArgs(MAINCFG, argc, argv);
    // First, initalize all of our GLM values
    mainState.model = new glm::mat4(1.0f);
    mainState.view = new glm::mat4(1.0f);
    mainState.projection = new glm::mat4(glm::perspective(glm::radians(mainState.mainArgs->getArguments()->FOV), (float) mainState.mainArgs->getArguments()->width/mainState.mainArgs->getArguments()->height, 0.1f, 1000.0f));
    mainState.ortho = new glm::mat4(glm::ortho(0.0f, 200.0f, 0.0f, 200.0f, 0.1f, 100.0f));
    mainState.mainPhysics = new PhysicsCore(physx::PxVec3(0.0f, -9.87, 0.0f));
    mainState.basicMaterial = new PhysicsMaterial(1.0f, 1.0f, 0.01f, mainState.mainPhysics);
    mainState.mainController = new PhysicsCapsuleController(mainState.basicMaterial, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 70.0f);

    // Now we can initalize our window, shaders, and world
    mainState.mainWindow = ((new Window(mainState.mainArgs->getArguments()->title.c_str(), mainState.mainArgs->getArguments()->width, mainState.mainArgs->getArguments()->height)));

    mainState.mainAudio = new AudioBase();

    mainState.mainShader = (new Shader("shaders/vertex.glsl", "shaders/fragment.glsl"));
    mainState.skyBox =  new Shader("shaders/skybox_v.glsl", "shaders/skybox_f.glsl");

    mainState.mainWorld = new World(mainState.mainShader, mainState.model, mainState.view, mainState.projection);
    mainState.mainBindings = new Bindings();
    mainState.mainCamera = mainState.mainWorld->createCamera("player_cam", glm::vec3(0.0f, 3.0f, 0.0f), PLAYER_WALK, mainState.view, mainState.mainWindow, mainState.mainController, mainState.mainPhysics);

    mainState.mainLoader = new WorldLoader(mainState.mainArgs->getArguments()->tMap.c_str(), mainState.mainShader, mainState.mainWorld, mainState.mainPhysics, mainState.mainCamera);

    gameState.currentCamera = mainState.mainCamera;
    gameState.currentShader = mainState.mainShader;
    gameState.currentWindow = mainState.mainWindow;
    gameState.currentWorld  = mainState.mainWorld;
    mainGame = (new GameMain(&mainState));

    mainState.mainSkybox = new CubeMap("clouds", GL_TEXTURE10, GL_RGB, 0, mainState.skyBox, mainState.view, mainState.projection);
    //mainWorld->addObject((Object*) mainSkybox, "skybox");

    start = 0;
    end = 0;
    fps = 0;
    setupBinds();

    mainState.mainWorld->useMain();

    mainState.mainWorld->cacheLocation();


    mainState.mainCamera->setDeltaTime(1.0f);
    start2 = getNanoseconds();
    ((GameMain*)mainGame)->gameInit();



    while (!mainState.mainWindow->getCloseState()){
        uint64_t start_hp = getNanoseconds();
        start = getNanoseconds();


        keys = mainState.mainWindow->getActiveKeys();
        Isrun = 0;
        for (unsigned int k = 0; k<keys.size(); k++)
        {
            mainState.mainBindings->callFunc(keys.at(k));
        }
        if (Isrun != 1){
            mainState.mainCamera->setSpeed(PLAYER_WALK);
        }

        mainState.mainCamera->getTarget(posHolder);
        mainState.mainController->move(posHolder);


        mainState.mainShader->useMain();
        mainState.mainShader->setFloat(mainState.mainShader->getUniformLocation("brightness"), mainState.mainArgs->getArguments()->brightness);

        ((GameMain*)mainGame)->gameLoop();

        mainState.mainWorld->updateMat();
        mainState.mainWorld->updateObjects();
        //mainCamera->update();
        mainState.mainSkybox->update();

        mainState.mainWindow->update();



        end = getNanoseconds();
        uint64_t end_hp = getNanoseconds();

        uint64_t time_span = end_hp-start_hp;

        std::this_thread::sleep_for(std::chrono::nanoseconds((getSecond()/FPS_MAX)-time_span));

        end = getNanoseconds();
        end_hp = getNanoseconds();

        time_span = end_hp-start_hp;

        mainState.mainPhysics->simulate( 1.0f / (float)(getSecond() / (time_span)) );


        mainState.mainController->getPosition(posHolder);
        mainState.mainCamera->setPos(glm::vec3(posHolder[0], posHolder[1], posHolder[2]));

        end = getNanoseconds();
        end_hp = getNanoseconds();

        time_span = end_hp-start_hp;


        fps++;
        if (end - start2 >= getSecond())
        {
            printf("\rFPS: %lu     ", fps);
            fflush(stdout);
            start2 = getNanoseconds();
            fps = 0;
        }

        mainState.mainCamera->setDeltaTime((float) time_span/std::chrono::nanoseconds((getSecond()/FPS_MAX)).count());

        fflush(stdout);
    }
    quit();
    return 0;
}

