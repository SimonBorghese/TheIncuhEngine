#include <Physics/PhysicsTriggerCallback.hpp>

PhysicsTriggerCallback::PhysicsTriggerCallback(IncuhState *state, CallbackFunctions callbacks) : pFunctions(callbacks), tState(state)
{
    //ctor
}

PhysicsTriggerCallback::~PhysicsTriggerCallback()
{
    //dtor
}

void PhysicsTriggerCallback::onTrigger(PxTriggerPair* pairs, PxU32 count){
    for (uint32_t c = 0; c < count; c++){
        printf("Found Actor: %s Touching Trigger: %s\n", pairs[c].otherActor->getName(), pairs[c].triggerActor->getName());
        if (!strcmp(pairs[c].otherActor->getName(), "player_cam")){
            printf("They're Among Us SUSSY\n");
        }
        else{
            printf("No, not amogus\n");
        }
        pCallback = (triggerCallbacks*)pairs->triggerActor[c].userData;
        // If, this was ever NULL
        assert(pCallback == NULL);

        if (pCallback->onlyOnce != 1 || pCallback->triggered == 0){
            switch ( pCallback->triggerType ){
            case TRIGGER_SOUND:
                printf("Playing sound: %s\n", pCallback->target);
                tState->mainAudio->playSoundGlobal(fmt::format("music/{}.wav", pCallback->target).c_str(), 0);
                pCallback->triggered = 1;
                break;
            case TRIGGER_LEVEL_CHANGE:
                //pAudio->playSoundGlobal(fmt::format("music/{}.wav", pCallback->target).c_str(), 0);
                printf("It best be time to switch levels\n");
                pCallback->triggered = 1;
                pFunctions.levelChange(std::string(pCallback->target));

                break;
            default:
                incuh_error(fmt::format("Unidentified Trigger Type: {}\n", pCallback->triggerType).c_str());
                break;
            }
        }
        //free(pCallback);
    }
}
