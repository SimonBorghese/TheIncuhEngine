#define CBSP_IMPLEMNT 1
#include <WorldLoader.hpp>

WorldLoader::WorldLoader(const char *fileName, Shader *shader, World *__target, PhysicsCore *core, Camera *cam) {

    incuh_info(fmt::format("Loading...: {}\n", fileName));

    std::string levelFile = fmt::format("maps/{}/level.bsp", fileName);
    incuh_info(fmt::format("Loading...: {}\n", levelFile.c_str()));

    bsp = CBSP_loadBSP(levelFile.c_str());
    float targetPos[3] = {0.0f, 0.0f, 0.0f};
    entityMap.insert(std::pair(std::string("prop"), PROP));
    entityMap.insert(std::pair(std::string("light"), LIGHT));
    entityMap.insert(std::pair(std::string("sun"), SUN));
    entityMap.insert(std::pair(std::string("trigger"), TRIGGER));
    for (int e = 0; e < bsp->nEntityCount; e++){
            incuh_info(fmt::format("found entity: {}\n", bsp->mEntity[e].classname));

        if (!strcmp(bsp->mEntity[e].classname, "info_player_start")){
                CBSP_getOriginFromEntity(&bsp->mEntity[e], &(targetPos[0]));
                //incuh_info(fmt::format("Found player pos: {} {} {}\n", targetPos[0], (targetPos[2]), targetPos[1]).c_str());
                //incuh_info(fmt::format("Found real player pos: {} {} {}\n", targetPos[1]*BSPSCALE, (targetPos[2]*BSPSCALE), targetPos[0]*BSPSCALE).c_str());
                cam->setPos(glm::vec3(targetPos[0]*BSPSCALE, (targetPos[2]*BSPSCALE), targetPos[1]*BSPSCALE ));
        }
        else if (!strcmp(bsp->mEntity[e].classname, "worldspawn")){
                incuh_info(fmt::format("Playing: music/{}.wav\n", CBSP_getKeyFromEntity(&bsp->mEntity[e], "music")));
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
                switch (entityMap.at(entType)){
                    case PROP:
                        goto PROP_SPAWN;
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
                    dynamic_prop = atoi(CBSP_getKeyFromEntity(&bsp->mEntity[e], "dynamic"));
                    // If a name cannot be found...
                    if (!strcmp( CBSP_getKeyFromEntity(&bsp->mEntity[e], "name"), CBSP_getKeyFromEntity_FAILURE )){
                        __target->createObject(fmt::format("UNNAMED:{}:{}", bsp->mEntity[e].classname, e).c_str(),
                            glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                            glm::quat(0.7071068f, 0.7071068f, 0.0f, 0.0f),
                            glm::vec3((double) j["scale"]),
                            CBSP_getKeyFromEntity(&bsp->mEntity[e], "prop"),
                            NULL,
                            new Material(shader, "materials/default.json"), 0, core, dynamic_prop);
                    } else{ // If a name is found...
                        __target->createObject( CBSP_getKeyFromEntity(&bsp->mEntity[e], "name"),
                            glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                            glm::quat(0.7071068f, 0.7071068f, 0.0f, 0.0f),
                            glm::vec3((double) j["scale"]),
                            CBSP_getKeyFromEntity(&bsp->mEntity[e], "prop"),
                            NULL,
                            new Material(shader, "materials/default.json"), 0, core, dynamic_prop);
                    }
                continue;
                // END PROP

                LIGHT_SPAWN:
                    lightdistance = atoi(CBSP_getKeyFromEntity(&bsp->mEntity[e], "distance"));
                    intensity = atoi(CBSP_getKeyFromEntity(&bsp->mEntity[e], "intensity"));
                    lightdistance *= 8;
                    assert(lightdistance == 0);
                    assert(intensity == 0);
                    // If a name cannot be found...
                    if (!strcmp( CBSP_getKeyFromEntity(&bsp->mEntity[e], "name"), CBSP_getKeyFromEntity_FAILURE )){
                        __target->createPointLight(fmt::format("{}:{}", bsp->mEntity[e].classname, e).c_str(),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f),
                                                   1.0f, (4.5f/lightdistance), (75.0f / (lightdistance*lightdistance)));
                    } else{ // If a name is found...
                        __target->createPointLight(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name"),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f),
                                                   1.0f, (4.5f/lightdistance), (75.0f / (lightdistance*lightdistance)));
                    }
                continue;
                //END LIGHT

                SUN_SPAWN:
                    intensity = atoi(CBSP_getKeyFromEntity(&bsp->mEntity[e], "intensity"));
                    assert(intensity == 0);
                    // If a name cannot be found...
                    if (!strcmp( CBSP_getKeyFromEntity(&bsp->mEntity[e], "name"), CBSP_getKeyFromEntity_FAILURE )){
                        __target->createDirectionalLight(fmt::format("{}:{}", bsp->mEntity[e].classname, e).c_str(),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f));
                    } else{ // If a name is found...
                        __target->createDirectionalLight(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name"),
                                                   glm::vec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                                                   new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f), new glm::vec3((float) intensity/100.0f));
                    }
                continue;
                //END SUN

                SOUND_TRIGGER:
                    // We really don't need to make a dedicated object for this
                    triggerCallbacks mainCallback;
                    mainCallback.triggerType = TRIGGER_SOUND;
                    mainCallback.soundName = CBSP_getKeyFromEntity(&bsp->mEntity[e], "sound");
                    core->addPhysicsObject(new PhysicsTrigger(CBSP_getKeyFromEntity(&bsp->mEntity[e], "name"),
                            physx::PxVec3(targetPos[0] * BSPSCALE, (targetPos[2] * BSPSCALE), -targetPos[1]*BSPSCALE),
                            (float) atoi(CBSP_getKeyFromEntity(&bsp->mEntity[e], "distance")),
                            new PhysicsMaterial((0.0f), (0.0f), (0.0f), core), core,
                            &mainCallback));

                continue;
                //END TRIGGER SPAWN

            }
            else{
                incuh_error(fmt::format("Unidentified Entity: {}\n", bsp->mEntity[e].classname));
            }
        }
    }

    //free(bsp);
    __target->createObject( "world2",
                            glm::vec3(0.0f, 0.0f, 0.0f),
                            glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                            glm::vec3((BSPSCALE)),
                            fileName,
                            NULL,
                            new Material(shader, "materials/default.json"), 1, core);

}

void WorldLoader::update(){
}

WorldLoader::~WorldLoader(){
    if (bsp != nullptr){
        free(bsp);
    }
}
