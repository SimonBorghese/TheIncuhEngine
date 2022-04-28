#include <Material.hpp>

Material::Material(Shader *shader, const char *fileName) : __shader(shader) {
  std::ifstream file(fileName);
  nlohmann::json basefile;
  file >> basefile;
  file.close();


  __ambient = new glm::vec3(0.0f);
  __ambient->x = (float) basefile["ambient"]["x"];
  __ambient->y = (float) basefile["ambient"]["y"];
  __ambient->z = (float) basefile["ambient"]["z"];

  __diffuse = new glm::vec3(0.0f);
  __diffuse->x = (float) basefile["diffuse"]["x"];
  __diffuse->y = (float) basefile["diffuse"]["y"];
  __diffuse->z = (float) basefile["diffuse"]["z"];

  __specular = new glm::vec3(0.0f);
  __specular->x = (float) basefile["specular"]["x"];
  __specular->y = (float) basefile["specular"]["y"];
  __specular->z = (float) basefile["specular"]["z"];

  __shine = (float) basefile["shine"];

  __ambient_loc = __shader->getUniformLocation("material.ambient");
  __diffuse_loc = __shader->getUniformLocation("material.diffuse");
  __specular_loc = __shader->getUniformLocation("material.specular");
  __shine_loc = __shader->getUniformLocation("material.shininess");
}

Material::~Material(){
    delete __ambient;
    delete __diffuse;
    delete __specular;


}

void Material::apply(){
  __shader->setVec3(__ambient_loc, *__ambient);
  __shader->setVec3(__diffuse_loc, *__diffuse);
  __shader->setVec3(__specular_loc, *__specular);
  __shader->setFloat(__shine_loc, __shine);
}
