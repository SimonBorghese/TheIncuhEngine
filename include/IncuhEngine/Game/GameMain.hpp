#ifndef GAMEMAIN_HPP
#define GAMEMAIN_HPP
#include <Window.hpp>
#include <Shader.hpp>
#include <World.hpp>
#include <Camera.hpp>
#include <Light.hpp>
#include <GameState.h>
#include <NPC/n_baseNPC.hpp>
#include <Physics/PhysicsBox.hpp>
#include <vector>
#include <Game/GameObject.hpp>
#include <Game/GameTransform.hpp>
#include <Game/g_goodObject.hpp>

typedef struct{
    World   *currentWorld;
    Shader  *currentShader;
    Window *currentWindow;
    Camera  *currentCamera;
} IE_GameState;

class GameMain{
    public:
        GameMain(IncuhState *state);
        virtual ~GameMain();
        void gameInit();
        void gameLoop();
        void gameClose();
    private:
        IncuhState *gState;
        std::vector<Incuh::GameObject*> gameObjects;

        Incuh::g_goodObject *good;

};

#endif // GAMEMAIN_HPP
