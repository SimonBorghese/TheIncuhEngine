#ifndef STDSCALE_H_INCLUDED
#define STDSCALE_H_INCLUDED
/*
 * The purpose of this file is to contain structs & defines needed by multiple files that also don't contain any dependancies on non-built in types
*/
#include <functional>


#define GLOBAL_BSP_SCAlE 0.0625 // 1/16
#define FPS_MAX 400
#define PLAYER_WALK 15.0f
#define PLAYER_RUN 0.1f

enum TRIGGER_TYPES{
    TRIGGER_SOUND = 0,
    TRIGGER_LEVEL_CHANGE = 1,
    TRIGGER_SCRIPTED = 2
};

typedef struct{
    int triggerType;
    const char *target;
    const char *script;
    unsigned int onlyOnce : 1;
    unsigned int triggered : 1;
} triggerCallbacks;



#endif // STDSCALE_H_INCLUDED
