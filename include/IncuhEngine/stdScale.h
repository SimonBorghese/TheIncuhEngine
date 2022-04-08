#ifndef STDSCALE_H_INCLUDED
#define STDSCALE_H_INCLUDED

#define GLOBAL_BSP_SCAlE 0.125 // 1/8

enum TRIGGER_TYPES{
    TRIGGER_SOUND = 0
};

typedef struct{
    int triggerType;
    const char *soundName;
} triggerCallbacks;



#endif // STDSCALE_H_INCLUDED
