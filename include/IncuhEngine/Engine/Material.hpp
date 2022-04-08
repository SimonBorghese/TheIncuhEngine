#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include <glm/glm.hpp>
#include <Shader.hpp>
#ifndef NOEXERNLIBS

#include <nlohmann/json.hpp>
#include <fstream>
#endif

class Material{
  public:
    Material(Shader *shader, const char *fileName);

    virtual ~Material();

    void apply();
  private:
    Shader *__shader;
    glm::vec3 *__ambient, *__diffuse, *__specular;
    uint32_t __ambient_loc, __diffuse_loc, __specular_loc, __shine_loc;
    float __shine;
};

#endif
