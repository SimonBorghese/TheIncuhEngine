#include <World.hpp>
#include <stdio.h>

World::World(Shader *shader, glm::mat4 *model, glm::mat4 *view, glm::mat4 *projection) : __shader(shader), __model(model), __view(view), __projection(projection), __lightCount(0), model_loader(nullptr) {
}

World::~World(){

}

void World::cacheLocation(){
  __model_loc = __shader->getUniformLocation("model");
  __view_loc = __shader->getUniformLocation("view");
  __projection_loc = __shader->getUniformLocation("projection");
  __lightCount_loc = __shader->getUniformLocation("lightCount");
}
void World::updateMat(){
  //__shader->setMatrix4f(__model_loc, *__model);
  __shader->setMatrix4f(__view_loc, *__view);
  __shader->setMatrix4f(__projection_loc, *__projection);
}
void World::useMain(){
  __shader->useMain();
}
Shader *World::getShader(){
  return __shader;
}
uint32_t World::getUniformLoc(const char *name){
  return __shader->getUniformLocation(name);
}

void World::setVec3(uint32_t loc, glm::vec3 vector){
  __shader->setVec3(loc, vector);
}

void World::addObject(Object *obj, const char *name){
    __world_objects.push_back((Object*) obj);
	mWorldObjects.insert(std::pair<std::string, Object*>(std::string(name), (Object*) obj));
}

Model* World::createObject( const char *name, glm::vec3 pos, glm::quat rot, glm::vec3 scle, const char *fileName, Image *diffuse, Material *mat, int useCollider, PhysicsCore *core, int dynamic){
    Model *newModel = new Model(name, pos, rot, scle, fileName, __shader, diffuse, mat, useCollider, core, dynamic);
	__world_objects.push_back((Object*) newModel);
	mWorldObjects.insert(std::pair<std::string, Object*>(std::string(name), (Object*) newModel));
	return newModel;
}

Light* World::createPointLight( const char *name, glm::vec3 pos, glm::vec3 *ambient, glm::vec3 *diffuse, glm::vec3 *specular, float const_val, float linear_val, float quadratic_val){
	Light *newLight = new Light( pos, ambient, diffuse, specular, const_val, linear_val, quadratic_val, __shader, POINT_LIGHT, __lightCount++);
	__world_objects.push_back((Object*) newLight);
    mWorldObjects.insert(std::pair<std::string, Object*>(std::string(name), (Object*) newLight));
	return newLight;
}

Light* World::createDirectionalLight( const char *name, glm::vec3 direction, glm::vec3 *ambient, glm::vec3 *diffuse, glm::vec3 *specular){
    Light *newLight = new Light( direction, ambient, diffuse, specular, 0.0, 0.0, 0.0, __shader, DIRECTIONAL, __lightCount++);
	__world_objects.push_back((Object*) newLight);
    mWorldObjects.insert(std::pair<std::string, Object*>(std::string(name), (Object*) newLight));
	return newLight;
}

Camera* World::createCamera( const char *name, glm::vec3 newCameraPos, float speed, glm::mat4 *view_mat, Window *win, PhysicsController *control, PhysicsCore *core){
    Camera *newCam = new Camera(newCameraPos, speed, __shader, view_mat, win, control);
    __world_objects.push_back((Object*) newCam);
    mWorldObjects.insert(std::pair<std::string, Object*>(std::string(name), (Object*) newCam));
    float currentPos[3];
    currentPos[0] = newCameraPos.x;
    currentPos[1] = newCameraPos.y;
    currentPos[2] = newCameraPos.z;
    core->addPhysicsController(control, currentPos);

    control->setName(name);
    return newCam;
}

void World::updateObjects(){
  __shader->setInt(__lightCount_loc, __lightCount);
	for (unsigned int o = 0; o < __world_objects.size(); o++){
        __shader->useMain();
        __world_objects.at(o)->update();
  }
}

glm::vec3 World::getGravity(){
    //return glm::vec3(py__dynamicsWorld->getGravity().getX(), py__dynamicsWorld->getGravity().getY(), py__dynamicsWorld->getGravity().getZ());
    return glm::vec3(0.0f);
}

Object* World::getObjectByName(const char *name){
	for (auto const& i : mWorldObjects) {
	    if (!strcmp(i.first.c_str(), name)){
	    	return i.second;
	    }
	}
	incuh_warning(fmt::format("Unable to locate object name: {}\n", name).c_str());
	return NULL;

}
