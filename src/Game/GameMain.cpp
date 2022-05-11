#include <Game/GameMain.hpp>

GameMain::GameMain(IncuhState *state) : gState(state) {}

GameMain::~GameMain() {}

void GameMain::gameInit(){
    good = new Incuh::g_goodObject(gState);

    classMap.insert(std::make_pair( std::string("g_goodObject"), good));

    //if (gState->mainWorld->getObjectByName("box1")) {
    //    good->addModel(((Model*) gState->mainWorld->getObjectByName("box1")));
    //}
    gameObjects.push_back(good);

    for (unsigned int g = 0; g < gameObjects.size(); g++){
        gameObjects.at(g)->startObjects();
    }
}

void GameMain::gameLoop(){
    if (gState->mainCamera->getPos().y < -50){
        gState->mainCamera->setPos(glm::vec3(0.0f, 10.0f, 0.0f));
    }

    for (unsigned int g = 0; g < gameObjects.size(); g++){
        gameObjects.at(g)->updateObjects();
    }

}

void GameMain::gameClose(){
    //free(playerLight);
    /*
    for (unsigned int g = 0; g < gameObjects.size(); g++){
        delete gameObjects.at(g);
    }
    for (std::pair<std::string, Incuh::GameObject*> obj: classMap){
        delete obj.second;
    }
     */
}

void GameMain::addModelToClass(Model* model, std::string classname){
    //auto pos = classMap.find(classname);
    if (classMap.count(classname) > 0){
        classMap.at(classname)->addModel(model);
    }
    else{
        printf("Warning: Class name: %s is not a registered class\n", classname.c_str());
    }
}

Incuh::GameObject* GameMain::getGameObjectFromClassname(std::string classname){
    if (classMap.count(classname) > 0){
        return classMap.at(classname);
    }
    else{
        printf("Warning: Class name: %s is not a registered class\n", classname.c_str());
        return NULL;
    }
}