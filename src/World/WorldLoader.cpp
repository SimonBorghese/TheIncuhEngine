#define CBSP_IMPLEMNT 1
#include <WorldLoader.hpp>
WorldLoader::WorldLoader(IncuhState *state){
    incuh_info(fmt::format("Loading...: {}\n", state->mainArgs->getArguments()->tMap.c_str()));

    std::string levelFile = fmt::format("maps/{}/level.bsp", state->mainArgs->getArguments()->tMap.c_str());
    incuh_info(fmt::format("Loading...: {}\n", levelFile.c_str()));

    bsp = CBSP_loadBSP(levelFile.c_str());
    float targetPos[3] = {0.0f, 0.0f, 0.0f};
    entityMap.insert(std::make_pair(std::string("prop"), PROP));
    entityMap.insert(std::make_pair(std::string("prop_script"), PROP_SCRIPT));
    entityMap.insert(std::make_pair(std::string ("light"), LIGHT));
    entityMap.insert(std::make_pair(std::string ("sun"), SUN));
    entityMap.insert(std::make_pair(std::string ("trigger"), TRIGGER));
    for (int e = 0; e < bsp->nEntityCount; e++){
            incuh_info(fmt::format("found entity: {}\n", bsp->mEntity[e].classname));

        if (!strcmp(bsp->mEntity[e].classname, "info_player_start")){
                CBSP_getOriginFromEntity(&bsp->mEntity[e], &(targetPos[0]));
                printf("Balls : %f %f %f\n",targetPos[0]*BSPSCALE, (targetPos[2]*BSPSCALE), targetPos[1]*BSPSCALE);
                state->mainCamera->setPos(glm::vec3(targetPos[1]*BSPSCALE, (targetPos[2]*BSPSCALE), targetPos[0]*BSPSCALE ));

                //PLAYER_WALK *= atof(CBSP_getKeyFromEntity(&bsp->mEntity[e], "speed"));
                //PLAYER_RUN *= atof(CBSP_getKeyFromEntity(&bsp->mEntity[e], "speed"));
                state->mainCamera->setSpeed( state->mainCamera->getSpeed() * atof(  CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "speed"))  ));
        }
        else if (!strcmp(bsp->mEntity[e].classname, "worldspawn")){
            incuh_info(fmt::format("Playing: music/{}.wav\n", ( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "music")) )));
            //wState->mainAudio->playSoundGlobal(fmt::format("music/{}.wav", CBSP_getKeyFromEntity(&bsp->mEntity[e], "music")).c_str(), 1);
        }
        else{
            std::ifstream entFile(fmt::format("entities/{}.ent", bsp->mEntity[e].classname).c_str());
            std::string objName;
            nlohmann::json j;
            try{
                entFile >> j;
                objName = j["name"];
            }
            catch (...){
                objName = "ERROR";
            }
            // If the entity file is found
            if (strcmp(objName.c_str(), "ERROR") != 0){
                CBSP_getOriginFromEntity(&bsp->mEntity[e], &(targetPos[0]));

                std::string entType = j["type"];

                // WHY DON'T YOU HAVE STRING SWITCHES C++
                // If the object is a prop
                int dynamic_prop = 0;
                int lightdistance = 0;
                int intensity = 0;
                int triggerType = 0;

                char *charHolder = nullptr;
                triggerCallbacks *triggerCallback = nullptr;
                switch (entityMap.at(entType)){
                    case PROP:
                        goto PROP_SPAWN;
                        break;
                    case PROP_SCRIPT:
                        goto PROP_SCRIPT_SPAWN;
                        break;
                    case LIGHT:
                        goto LIGHT_SPAWN;
                        break;
                    case SUN:
                        goto SUN_SPAWN;
                        break;
                    case TRIGGER:
                        triggerType = (int) j["trigger_type"];
                        switch (triggerType){
                            case TRIGGER_SOUND:
                                goto SOUND_TRIGGER;
                                break;
                            case TRIGGER_LEVEL_CHANGE:
                                goto LEVEL_CHANGE_TRIGGER;
                                break;
                            case TRIGGER_SCRIPTED:
                                goto SCRIPTED_TRIGGER;
                                break;
                            default:
                                incuh_error(fmt::format("Unidentified Trigger Type: {}\n", triggerType ).c_str());
                                break;
                        }
                        break;
                    default:
                        incuh_error(fmt::format("Unidentified Entity Type: {}\n", entType.c_str()));
                        continue;
                        break;
                }

                PROP_SPAWN:
                    charHolder = (char*) CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "dynamic"));
                    dynamic_prop = atoi(charHolder);
                    //free(charHolder);
                    // If a name cannot be found
                    printf("Spawning...: %s\n", fmt::format("UNNAMED:{}:{}", bsp->mEntity[e].classname, e).c_str());
                    if (!strcmp( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")), CBSP_getKeyFromEntity_FAILURE )){
                        state->mainWorld->createObject(fmt::format("UNNAMED:{}:{}", bsp->mEntity[e].classname, e).c_str(),
                            glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                            glm::quat(0.7071068f, 0.7071068f, 0.0f, 0.0f),
                            glm::vec3((double) j["scale"]),
                            CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "prop")),
                            NULL,
                            (Material*) (new Material(state->mainShader, "materials/default.json")), 0, dynamic_prop);
                    } else{ // If a name is found...
                        state->mainWorld->createObject( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")),
                            glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                            glm::quat(0.7071068f, 0.7071068f, 0.0f, 0.0f),
                            glm::vec3((double) j["scale"]),
                            CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "prop")),
                            NULL,
                            (Material*) (new Material(state->mainShader, "materials/default.json")), 0, dynamic_prop);
                    }
                continue;
                // END PROP


                PROP_SCRIPT_SPAWN:
                charHolder = (char*) CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "dynamic"));
                dynamic_prop = atoi(charHolder);
                //free(charHolder);
                // If a name cannot be found
                printf("Spawning...: %s\n", fmt::format("UNNAMED:{}:{}", bsp->mEntity[e].classname, e).c_str());
                if (!strcmp( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")), CBSP_getKeyFromEntity_FAILURE )){
                    state->mainGame->addModelToClass(state->mainWorld->createObject(fmt::format("UNNAMED:{}:{}", bsp->mEntity[e].classname, e).c_str(),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   glm::quat(0.7071068f, 0.7071068f, 0.0f, 0.0f),
                                                   glm::vec3((double) j["scale"]),
                                                   CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "prop")),
                                                   NULL,
                                                   (Material*) (new Material(state->mainShader, "materials/default.json")), 0, dynamic_prop),
                                                     std::string((char*) CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "script"))));
                } else{ // If a name is found...
                    state->mainGame->addModelToClass(state->mainWorld->createObject( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")),
                                                    glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                    glm::quat(0.7071068f, 0.7071068f, 0.0f, 0.0f),
                                                    glm::vec3((double) j["scale"]),
                                                    CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "prop")),
                                                    NULL,
                                                    (Material*) (new Material(state->mainShader, "materials/default.json")), 0, dynamic_prop),
                                                     std::string((char*) CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "script"))));
                }
                continue;

                LIGHT_SPAWN:
                    lightdistance = atoi(CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "distance")));
                    intensity = atoi(CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "intensity")));
                    lightdistance *= 8;
                    assert(lightdistance == 0);
                    assert(intensity == 0);
                    // If a name cannot be found...
                    if (!strcmp( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")), CBSP_getKeyFromEntity_FAILURE )){
                        state->mainWorld->createPointLight(fmt::format("{}:{}", bsp->mEntity[e].classname, e).c_str(),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f),
                                                   1.0f, (4.5f/lightdistance), (75.0f / (lightdistance*lightdistance)));
                    } else{ // If a name is found...
                        state->mainWorld->createPointLight( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f),
                                                   1.0f, (4.5f/lightdistance), (75.0f / (lightdistance*lightdistance)));
                    }
                continue;
                //END LIGHT

                SUN_SPAWN:
                    intensity = atoi( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "intensity")) );
                    assert(intensity == 0);
                    // If a name cannot be found...
                    if (!strcmp( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")), CBSP_getKeyFromEntity_FAILURE )){
                        state->mainWorld->createDirectionalLight(fmt::format("{}:{}", bsp->mEntity[e].classname, e).c_str(),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f));
                    } else{ // If a name is found...
                        state->mainWorld->createDirectionalLight( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f));
                    }
                continue;
                //END SUN

                SOUND_TRIGGER:
                    // We really don't need to make a dedicated object for this
                    triggerCallback = (triggerCallbacks*) GB(new triggerCallbacks);
                    triggerCallback->triggerType = TRIGGER_SOUND;
                    triggerCallback->target = CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "sound"));
                    triggerCallback->onlyOnce = !(atoi( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "only_once"))));
                    triggerCallback->triggered = 0;
                    state->mainPhysics->addPhysicsObject((PhysicsTrigger*) GB(new PhysicsTrigger(
                                                             CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")),
                            physx::PxVec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                            (float) atoi(CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "distance"))),
                            (PhysicsMaterial*) GB(new PhysicsMaterial((0.0f), (0.0f), (0.0f), state->mainPhysics)), state->mainPhysics,
                            triggerCallback)));
                    //delete triggerCallback;

                continue;

                LEVEL_CHANGE_TRIGGER:
                    // We really don't need to make a dedicated object for this
                    triggerCallback = (triggerCallbacks*) GB(new triggerCallbacks);
                    triggerCallback->triggerType = TRIGGER_LEVEL_CHANGE;
                    triggerCallback->target = CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "level"));
                    triggerCallback->onlyOnce = !(atoi(std::string(CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "only_once"))).c_str()));
                    triggerCallback->triggered = 0;
                    state->mainPhysics->addPhysicsObject((PhysicsTrigger*) GB(new PhysicsTrigger( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")),
                            physx::PxVec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                            (float) atoi( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "distance")) ),
                            (PhysicsMaterial*) GB(new PhysicsMaterial((0.0f), (0.0f), (0.0f), state->mainPhysics)), state->mainPhysics,
                            triggerCallback)));
                    //delete triggerCallback;

                continue;

                SCRIPTED_TRIGGER:
                triggerCallback = (triggerCallbacks*) GB(new triggerCallbacks);
                triggerCallback->triggerType = TRIGGER_SCRIPTED;
                triggerCallback->script = (CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "script")));
                printf("found the funny script: %s\n", triggerCallback->script);
                triggerCallback->onlyOnce = !(atoi(std::string(CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "only_once"))).c_str()));
                triggerCallback->triggered = 0;
                state->mainPhysics->addPhysicsObject((PhysicsTrigger*) GB(new PhysicsTrigger( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name")),
                                                                                              physx::PxVec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                                                              (float) atoi( CB(CBSP_getKeyFromEntity(&bsp->mEntity[e], "distance")) ),
                                                                                              (PhysicsMaterial*) GB(new PhysicsMaterial((0.0f), (0.0f), (0.0f), state->mainPhysics)), state->mainPhysics,
                                                                                              triggerCallback)));
                continue;
                //END TRIGGER SPAWN

            }
            else{
                incuh_error(fmt::format("Unidentified Entity: {}\n", bsp->mEntity[e].classname));
            }
        }
    }
    state->mainWorld->createObject( "world2",
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                            glm::vec3((BSPSCALE)),
                            state->mainArgs->getArguments()->tMap.c_str(),
                            NULL,
                            (Material*) (new Material(state->mainShader, "materials/default.json")), 1);

}

void WorldLoader::update(){
}

WorldLoader::~WorldLoader(){
    KillCB();
    if (bsp != nullptr){
        CBSP_Destroy(bsp);
    }
}
