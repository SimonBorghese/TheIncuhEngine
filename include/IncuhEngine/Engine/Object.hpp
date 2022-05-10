#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Object{
  public:
    virtual ~Object() {}
    virtual void update() = 0;

    void translate(float x, float y, float z){
      __pos = new glm::vec3(x,y,z);
    }
    glm::vec3 getPosition() { return *__pos; }

    void rotate(float q, float x, float y, float z){
        __rot = new glm::quat(q, x,y,z);
    }
    glm::quat getRotation() { return *__rot; }

    void scale(float x, float y, float z){
        __scl = new glm::vec3(x,y,z);
    }
    glm::vec3 getScale() { return *__scl; }

  protected:
    glm::vec3 *__pos;
    glm::quat *__rot;
    glm::vec3 *__scl;

};

#endif
