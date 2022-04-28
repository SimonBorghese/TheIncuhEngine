#include <Light.hpp>

Light::Light(glm::vec3 pos, glm::vec3 *ambient, glm::vec3 *diffuse, glm::vec3 *specular, float const_val, float linear_val, float quadratic_val, Shader *shader, Types type, uint32_t lightCount) : __light_ambi(ambient), __light_diff(diffuse), __light_spec(specular), __const_val(const_val), __linear(linear_val), __quadratic(quadratic_val), __shader(shader) {
	// __pos is a protected member, thus, it cannot be used in initalization lists
    __pos = new glm::vec3(pos);
    switch (type){
        case POINT_LIGHT:
            __pos__pos = __shader->getUniformLocation(fmt::format("light[{}].lightPos",lightCount).c_str());
            break;
        case DIRECTIONAL:
            __pos__pos = __shader->getUniformLocation(fmt::format("light[{}].direction",lightCount).c_str());
            break;
        default:
            __pos__pos = __shader->getUniformLocation(fmt::format("light[{}].lightPos",lightCount).c_str());
            break;

    }
	// Cache all of our Uniform positions to help with performance

	__ambient_pos = __shader->getUniformLocation(fmt::format("light[{}].ambient",lightCount).c_str());
	__diffuse_pos = __shader->getUniformLocation(fmt::format("light[{}].diffuse",lightCount).c_str());
	__specular_pos = __shader->getUniformLocation(fmt::format("light[{}].specular",lightCount).c_str());

	__const_pos = __shader->getUniformLocation(fmt::format("light[{}].constant",lightCount).c_str());
	__linear_pos = __shader->getUniformLocation(fmt::format("light[{}].linear",lightCount).c_str());
	__quadratic_pos = __shader->getUniformLocation(fmt::format("light[{}].quadratic",lightCount).c_str());
}

Light::~Light(){
    delete __light_ambi;
    delete __light_diff;
    delete __light_spec;
    delete __pos;
}

void Light::update(){
	// Set all of our light values for each stage
  __shader->setVec3(__ambient_pos, *__light_ambi);
  __shader->setVec3(__diffuse_pos, *__light_diff);
  __shader->setVec3(__specular_pos, *__light_spec);

	// Setup all of our values for the attenuation values
  __shader->setFloat(__const_pos, __const_val);
  __shader->setFloat(__linear_pos, __linear);
  __shader->setFloat(__quadratic_pos, __quadratic);

	// Set our light position
  __shader->setVec3(__pos__pos, *__pos);
}
