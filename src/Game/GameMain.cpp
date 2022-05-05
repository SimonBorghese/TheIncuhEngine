#include <Game/GameMain.hpp>

GameMain::GameMain(IncuhState *state) : gState(state) {}

GameMain::~GameMain() {}

void GameMain::gameInit(){
    good = new Incuh::g_goodObject();
    if (gState->mainWorld->getObjectByName("UNNAMED:info_prop:5")) {
        good->addModel(((Model*) gState->mainWorld->getObjectByName("UNNAMED:info_prop:5")));
    }
    gameObjects.push_back(good);

    for (unsigned int g = 0; g < gameObjects.size(); g++){
        gameObjects.at(g)->start();
    }
}

void GameMain::gameLoop(){
    if (gState->mainCamera->getPos().y < -50){
        gState->mainCamera->setPos(glm::vec3(0.0f, 10.0f, 0.0f));
    }

    for (unsigned int g = 0; g < gameObjects.size(); g++){
        gameObjects.at(g)->update();
    }

}

void GameMain::gameClose(){
    //free(playerLight);
    for (unsigned int g = 0; g < gameObjects.size(); g++){
        delete gameObjects.at(g);
    }
}
