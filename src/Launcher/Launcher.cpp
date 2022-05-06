
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
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#define NDEBUG 1
#include <GameState.h>
#define CBSP_IMPLEMNT 1
#include <BSP/BSPMesh.hpp>
#include <CubeMap.hpp>

#define MAINCFG "cfg/launch.json"

IE_GameState        gameState;

IncuhState mainState;
GameMain *mainGame;

uint64_t profileTime;


std::vector<Object*> objects;

std::vector<uint8_t> keys;

uint64_t start, start2, end, fps;

float posHolder[3];

CallbackFunctions mainFuncs;

int bound = 0;
PhysicsPointer *boundTo;

void changeMap();

std::string newLevel;
uint8_t markedForDeath = 0;


uint64_t getNanoseconds(){
    auto time_hp = std::chrono::high_resolution_clock::now();
    return (uint64_t) (std::chrono::duration_cast<std::chrono::nanoseconds>(time_hp.time_since_epoch()).count());
}

uint64_t getSecond(){
    return (((std::chrono::duration_cast<std::chrono::nanoseconds>) (std::chrono::seconds(1))).count());
}


void quit(){
    mainState.mainWorld->destroyTheWorld();
    mainGame->gameClose();
	delete mainState.mainWindow;
	delete mainState.mainShader;

    //delete mainState.basicMaterial;
    delete mainState.mainController;
    //delete mainState.mainBindings;
    delete mainState.mainLoader;

    delete mainState.mainPhysics;
    exit(0);
}

void pickUp(){
    if (bound == 0){
        int multiplier = 3;
        float length = 20.0f;
    float pos[3] = { mainState.mainCamera->getPos().x + mainState.mainCamera->getForward().x * multiplier, mainState.mainCamera->getPos().y + mainState.mainCamera->getForward().y  * multiplier, mainState.mainCamera->getPos().z + mainState.mainCamera->getForward().z * multiplier};
    float dir[3] = {mainState.mainCamera->getForward().x, mainState.mainCamera->getForward().y, mainState.mainCamera->getForward().z };
    pos[1] += 1.5f;

    if (mainState.mainPhysics->rayCast(pos, dir, length) != NULL) {

        boundTo = mainState.mainPhysics->rayCast(pos, dir, length);
        boundTo->getPosition(dir);
        //printf("Found Piss: %f %f %f & offset %f %f %f\n", dir[0], dir[1], dir[2], pos[0] - dir[0], pos[1] - dir[1], pos[2] - dir[2]);
        bound = 1;
    }
    }
    else{
        bound = 0;
        boundTo = NULL;
    }

}


void setupBinds(){
    Bindings::setDownFunc(SDL_SCANCODE_W, std::bind(&Camera::moveForward, mainState.mainCamera));
    Bindings::setDownFunc(SDL_SCANCODE_S, std::bind(&Camera::moveBackward, mainState.mainCamera));
    Bindings::setDownFunc(SDL_SCANCODE_A, std::bind(&Camera::strafeLeft, mainState.mainCamera));
    Bindings::setDownFunc(SDL_SCANCODE_D, std::bind(&Camera::strafeRight, mainState.mainCamera));

  Bindings::setDownFunc(SDL_SCANCODE_UP, std::bind(&Camera::moveUp, mainState.mainCamera));
  Bindings::setDownFunc(SDL_SCANCODE_DOWN, std::bind(&Camera::moveDown, mainState.mainCamera));
  Bindings::setDownFunc(SDL_SCANCODE_LEFT, std::bind(&Camera::turnLeft, mainState.mainCamera));
  Bindings::setDownFunc(SDL_SCANCODE_RIGHT, std::bind(&Camera::turnRight, mainState.mainCamera));
  Bindings::setUpFunc(SDL_SCANCODE_ESCAPE, quit);
  Bindings::setUpFunc(SDL_SCANCODE_E, pickUp);
  Bindings::setUpFunc(SDL_SCANCODE_SPACE, std::bind(&Camera::jump, mainState.mainCamera));
  Bindings::setDownFunc(SDL_SCANCODE_LSHIFT, std::bind(&Camera::run, mainState.mainCamera));
  //Bindings::setFunc(SDL_SCANCODE_1, changeMap);
}

void changeMap(std::string targetMap){
    // If we run this right when the trigger calls back, we will die
    newLevel = targetMap;
    markedForDeath = 1;
}

void changeMapINTERNAL(){
    printf("LMAO\n");
    mainState.mainWorld->destroyTheWorld();
    delete mainState.mainController;
    //delete mainState.mainPhysics;
    mainState.mainPhysics->softCleanup();
    //delete mainState.basicMaterial;
    delete mainState.mainWorld;
    //delete mainState.mainBindings;
    //delete mainState.mainCamera;
    delete mainState.mainLoader;
    delete mainGame;
    delete mainState.mainSkybox;
    mainState.mainArgs->getArguments()->tMap = newLevel;
    //mainState.mainPhysics = new PhysicsCore(physx::PxVec3(0.0f, -9.87, 0.0f));
    //mainState.basicMaterial = new PhysicsMaterial(1.0f, 1.0f, 0.01f, mainState.mainPhysics);
    mainState.mainController = new PhysicsCapsuleController(new PhysicsMaterial(1.0f, 1.0f, 0.01f, mainState.mainPhysics), 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 70.0f);

    // Now we can initalize our window, shaders, and world
    mainState.mainPhysics->setTriggerCallback(new PhysicsTriggerCallback(&mainState, mainFuncs));


    mainState.mainWorld = new World(&mainState);
    //mainState.mainBindings = new Bindings();
    mainState.mainCamera = mainState.mainWorld->createCamera("player_cam", glm::vec3(0.0f, 0.0f, 0.0f), PLAYER_WALK, mainState.mainController);

    //mainState.mainLoader = new WorldLoader(mainState.mainArgs->getArguments()->tMap.c_str(), mainState.mainShader, mainState.mainWorld, mainState.mainPhysics, mainState.mainCamera);
    mainState.mainLoader = new WorldLoader(&mainState);
    gameState.currentCamera = mainState.mainCamera;
    gameState.currentShader = mainState.mainShader;
    gameState.currentWindow = mainState.mainWindow;
    gameState.currentWorld  = mainState.mainWorld;
    mainGame = new GameMain(&mainState);

    mainState.mainSkybox = new CubeMap("clouds", GL_TEXTURE16, GL_RGB, 0, mainState.skyBox, mainState.view, mainState.projection);

    start = 0;
    end = 0;
    fps = 0;
    setupBinds();
    mainState.mainWorld->useMain();

    mainState.mainWorld->cacheLocation();


    mainState.mainCamera->setDeltaTime(1.0f);
    start2 = getNanoseconds();
    ((GameMain*)mainGame)->gameInit();
}

int main(int argc, char **argv) {
    //mainFuncs.levelChange.swap();
    mainFuncs.levelChange = &changeMap;
    mainState.mainArgs = new LArgs::LauncherArgs(MAINCFG, argc, argv);
    // First, initalize all of our GLM values
    mainState.model = new glm::mat4(1.0f);
    mainState.view = new glm::mat4(1.0f);
    mainState.projection = new glm::mat4(glm::perspective(glm::radians(mainState.mainArgs->getArguments()->FOV), (float) mainState.mainArgs->getArguments()->width/mainState.mainArgs->getArguments()->height, 0.1f, 1000.0f));
    mainState.ortho = new glm::mat4(glm::ortho(0.0f, 200.0f, 0.0f, 200.0f, 0.1f, 100.0f));
    mainState.mainPhysics = new PhysicsCore(physx::PxVec3(0.0f, -9.87, 0.0f));
    //mainState.basicMaterial = new PhysicsMaterial(1.0f, 1.0f, 0.01f, mainState.mainPhysics);
    mainState.mainController = new PhysicsCapsuleController(new PhysicsMaterial(1.0f, 1.0f, 0.01f, mainState.mainPhysics), 1.5f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f, 70.0f);

    // Now we can initalize our window, shaders, and world
    mainState.mainWindow = new Window(&mainState);
    if (!mainState.mainWindow){
        return -1;
    }

    mainState.mainAudio = new AudioBase();

    mainState.mainPhysics->setTriggerCallback(new PhysicsTriggerCallback(&mainState, mainFuncs));

    mainState.mainShader = (new Shader("shaders/vertex.glsl", "shaders/fragment.glsl"));
    mainState.skyBox =  new Shader("shaders/skybox_v.glsl", "shaders/skybox_f.glsl");

    mainState.mainWorld = new World(&mainState);
    //mainState.mainBindings = new Bindings();
    mainState.mainCamera = mainState.mainWorld->createCamera("player_cam", glm::vec3(0.0f, 0.0f, 0.0f), PLAYER_WALK, mainState.mainController);

    //mainState.mainLoader = new WorldLoader(mainState.mainArgs->getArguments()->tMap.c_str(), mainState.mainShader, mainState.mainWorld, mainState.mainPhysics, mainState.mainCamera);
    mainState.mainLoader = new WorldLoader(&mainState);
    gameState.currentCamera = mainState.mainCamera;
    gameState.currentShader = mainState.mainShader;
    gameState.currentWindow = mainState.mainWindow;
    gameState.currentWorld  = mainState.mainWorld;
    mainGame = (new GameMain(&mainState));

    mainState.mainSkybox = new CubeMap("clouds", GL_TEXTURE16, GL_RGB, 0, mainState.skyBox, mainState.view, mainState.projection);
    //mainWorld->addObject((Object*) mainSkybox, "skybox");
    uint64_t end_hp = getNanoseconds();
    start = 0;
    end = 0;
    fps = 0;
    setupBinds();

    mainState.mainWorld->useMain();

    mainState.mainWorld->cacheLocation();


    mainState.mainCamera->setDeltaTime(1.0f);
    start2 = getNanoseconds();
    ((GameMain*)mainGame)->gameInit();

    uint64_t lastFrame = getNanoseconds();
    uint64_t currentFrame = getNanoseconds();

    bool showdemo;
    //uint32_t counter = 0;

    Image cursor("data/textures/cursor.png", GL_TEXTURE12, GL_RGBA, 1);

    //float offset[3];

    while (!mainState.mainWindow->getCloseState()){

        // BEGIN NEW FRAME


        //ImGui::ShowDemoWindow(&showdemo);
        uint64_t start_hp = getNanoseconds();
        start = getNanoseconds();
        ImGui::SetNextWindowPos(ImVec2((float) mainState.mainArgs->getArguments()->width/2 - 10.0f, (float) mainState.mainArgs->getArguments()->height/2 - 10.0f));
        ImGui::Begin("Hello, world!", &showdemo, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        /*
        ImGui::Text("This is some useful text.");


        if (ImGui::Button("Button")){
            //printf("I love fortnite\n");
            counter++;
        }
        ImGui::Text("counter = %u", counter);
        ImGui::Image((void*)(intptr_t)cursor.getTexture(), ImVec2(100,100));

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        */
        ImGui::Image((void*)(intptr_t)cursor.getTexture(), ImVec2(20,20));
        ImGui::End();


        if (markedForDeath){
            changeMapINTERNAL();
            markedForDeath=0;
        }


        keys = mainState.mainWindow->getActiveKeys();
        //for (unsigned int k = 0; k<keys.size(); k++)
        //{
        //    Bindings::callFunc(keys.at(k));
        //}
        mainState.mainCamera->getTarget(posHolder);
        mainState.mainController->move(posHolder);

        //printf("Pre Camera Pos: %f %f %f\n", mainState.mainCamera->getPos().x, mainState.mainCamera->getPos().y, mainState.mainCamera->getPos().z);

        if (bound){
            float target2[3];
            //mainState.mainCamera->getPos();
            target2[0] = mainState.mainCamera->getPos().x + (mainState.mainCamera->getForward().x*3);
            target2[1] = mainState.mainCamera->getPos().y + (mainState.mainCamera->getForward().y*3) + 1.5f;
            target2[2] = mainState.mainCamera->getPos().z + (mainState.mainCamera->getForward().z*3);
            boundTo->setPosition(target2);
        }


        mainState.mainShader->useMain();
        mainState.mainShader->setFloat(mainState.mainShader->getUniformLocation("brightness"), mainState.mainArgs->getArguments()->brightness);

        ((GameMain*)mainGame)->gameLoop();

        mainState.mainWorld->updateMat();
        mainState.mainWorld->updateObjects();
        //mainCamera->update();
        mainState.mainSkybox->update();

        //IMGUI
        //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //ImGui::Render();
        //END IM GUI
        mainState.mainWindow->update();

        //mainState.mainCamera->setDeltaTime((float) getNanoseconds()- end_hp);
        //printf("Time since last frame: (%lu %lu) (%lu %f)\n", end_hp, getNanoseconds(),getSecond(),  (float) ((getNanoseconds()/1000 - end_hp/1000) / getSecond()) );

        end = getNanoseconds();
        end_hp = getNanoseconds();

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
        currentFrame = getNanoseconds();
        //printf("Found: %lu %lu (%lu) ((%f)) \n", currentFrame, lastFrame, currentFrame-lastFrame, (currentFrame-lastFrame) / (float) getSecond());
        mainState.mainCamera->setDeltaTime((currentFrame-lastFrame) / (float) getSecond());
        lastFrame = getNanoseconds();



        fps++;
        if (end - start2 >= getSecond())
        {
            printf("\rFPS: %lu     ", fps);
            fflush(stdout);
            start2 = getNanoseconds();
            fps = 0;
        }



        fflush(stdout);
    }
    quit();
    return 0;
}

