#include <Game/GameMain.hpp>

GameMain::GameMain(IncuhState *state) : gState(state) {}

GameMain::~GameMain() {}

void GameMain::gameInit(){
    //box22 = new PhysicsBox(physx::PxVec3(0.0f, 0.0f, 0.0f), physx::PxVec3(5.0f), STATIC, gState->basicMaterial, gState->mainPhysics);
    //box22->setName("Triggered");
    //gState->mainPhysics->addPhysicsObject(box22);
}

void GameMain::gameLoop(){
    if (gState->mainCamera->getPos().y < -50){
        gState->mainCamera->setPos(glm::vec3(0.0f, 10.0f, 0.0f));
    }


}

void GameMain::gameClose(){
    //free(playerLight);
}
