#include <Physics/PhysicsTriggerCallback.hpp>

PhysicsTriggerCallback::PhysicsTriggerCallback()
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
        switch ( ((triggerCallbacks*)pairs->triggerActor[c].userData)->triggerType ){
            case TRIGGER_SOUND:
                printf("Playing sound: %s\n", ((triggerCallbacks*)pairs->triggerActor[c].userData)->soundName);
                break;
            default:
                incuh_error(fmt::format("Unidentified Trigger Type: {}\n", ((triggerCallbacks*)pairs->triggerActor[c].userData)->triggerType).c_str());
                break;
        }
    }
}
