#ifndef OBJECT_HPP
#define OBJECT_HPP
#include <glm/glm.hpp>

class Object{
  public:
    virtual void update() = 0;

    void translate(float x, float y, float z){
      __pos = new glm::vec3(x,y,z);
    }

  protected:
    glm::vec3 *__pos;
    glm::vec3 *__rot;
    glm::vec3 *__scl;

};

#endif
