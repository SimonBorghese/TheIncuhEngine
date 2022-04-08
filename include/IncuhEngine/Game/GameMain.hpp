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
        PhysicsBoxController *controller;

        Light *playerLight;
        Model *box;
        n_baseNPC *npc;

        glm::vec3 oldpos;

        PhysicsBox *box22;

};

#endif // GAMEMAIN_HPP
