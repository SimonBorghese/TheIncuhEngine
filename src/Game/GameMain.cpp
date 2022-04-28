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
    if (gState->mainWorld->getObjectByName("UNNAMED:info_prop:5")){
        //psrintf("%f %f %f\n", gState->mainWorld->getObjectByName("UNNAMED:info_prop:5")->getPosition().x, gState->mainWorld->getObjectByName("UNNAMED:info_prop:7")->getPosition().y, gState->mainWorld->getObjectByName("UNNAMED:info_prop:7")->getPosition().z);
        float direction[3];
        direction[0] = 0.0f;
        direction[1] = 0.0f;
        direction[2] = 1.0f;
        ((Model*) gState->mainWorld->getObjectByName("UNNAMED:info_prop:5"))->getPhysicsMesh()->applyForce(&direction[0], 20.0f);
    }

}

void GameMain::gameClose(){
    //free(playerLight);
}
