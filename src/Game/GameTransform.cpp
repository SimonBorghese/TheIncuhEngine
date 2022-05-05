#include <Game/GameTransform.hpp>

Incuh::Transform::Transform(float x, float y, float z, float q, float qw, float qy, float qz, float sx, float sy, float sz){
    position = glm::vec3(x,y,z);
    rotation = glm::quat(q, qw, qy, qz);
    scale = glm::vec3(sx, sy, sz);
}
Incuh::Transform::~Transform(){
    for (unsigned int x = 0; x < allocatedFloats.size(); x++){
        free(allocatedFloats.at(x));
    }
}

float* Incuh::Transform::glmToFloat(glm::vec3 val){
    float *targetFloat = (float*) malloc(sizeof(float) * 3);
    allocatedFloats.push_back(targetFloat);
    targetFloat[0] = val.x;
    targetFloat[1] = val.y;
    targetFloat[2] = val.z;
    return targetFloat;
}
float* Incuh::Transform::glmToFloat(glm::quat val){
    float *targetFloat = (float*) malloc(sizeof(float) * 4);
    allocatedFloats.push_back(targetFloat);
    targetFloat[0] = val.w;
    targetFloat[1] = val.x;
    targetFloat[2] = val.y;
    targetFloat[3] = val.z;
    return targetFloat;
}

glm::vec3 Incuh::Transform::getPosition(){ return position; }
glm::quat Incuh::Transform::getRotation(){ return rotation; }
glm::vec3 Incuh::Transform::getScale(){ return scale; }

void Incuh::Transform::setX( float x ){ position.x = x; }
void Incuh::Transform::setY( float y ){ position.y = y; }
void Incuh::Transform::setZ( float z ){ position.z = z; }

void Incuh::Transform::setRotationW( float w ) { rotation.w = w; }
void Incuh::Transform::setRotationX( float x ) { rotation.x = x; }
void Incuh::Transform::setRotationY( float y ) { rotation.y = y; }
void Incuh::Transform::setRotationZ( float z ) { rotation.z = z; }

void Incuh::Transform::setScaleX( float x ) { scale.x = x; }
void Incuh::Transform::setScaleY( float y ) { scale.y = y; }
void Incuh::Transform::setScaleZ( float z ) { scale.z = z; }

void Incuh::Transform::setPosition( float x, float y, float z ){ position = glm::vec3(x,y,z); }
void Incuh::Transform::setPosition( glm::vec3 newPos ){ position = newPos; }

void Incuh::Transform::setRotation( float q, float x, float y, float z ) { rotation = glm::quat(q,x,y,z); }
void Incuh::Transform::setRotation( glm::quat newPos ) { rotation = newPos; }

void Incuh::Transform::setScale( float x, float y, float z ) { scale = glm::vec3(x,y,z); }
void Incuh::Transform::setScale( glm::vec3 newPos ) { scale = newPos; }