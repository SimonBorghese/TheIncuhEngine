#ifndef LIGHT_HPP
#define LIGHT_HPP
#include <glm/glm.hpp>
#include <Shader.hpp>
#include <Object.hpp>
#ifndef NOEXERNLIBS

#include <fmt/core.h>
#include <fmt/format.h>
#endif
/*
The Light Class
*/


enum Types{
		POINT_LIGHT,
		DIRECTIONAL
};
class Light: public Object{
  public:
		// Point Light
    Light(glm::vec3 pos, glm::vec3 *ambient, glm::vec3 *diffuse, glm::vec3 *specular, float const_val, float linear_val, float quadratic_val, Shader *shader, Types type = POINT_LIGHT, uint32_t lightCount = 0);

    virtual ~Light();

    void update();

  private:
		// Stores the uniform position of each object
    uint32_t __pos__pos, __ambient_pos, __diffuse_pos, __specular_pos, __const_pos, __linear_pos, __quadratic_pos;

		// Stores vector info
    glm::vec3 *__light_pos;

		// Store our light info for each pass
		glm::vec3 *__light_ambi, *__light_diff, *__light_spec;

		// Stores our values for light fall off
		float __const_val, __linear, __quadratic;

		// Shader Object
    Shader *__shader;

};
#endif
