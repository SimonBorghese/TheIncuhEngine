#ifndef MESH_HPP
#define MESH_HPP
#include <vector>
#include <string>
#include <Object.hpp>
#include <Shader.hpp>
#include <Image.hpp>
#include <Physics/PhysicsMesh.hpp>
#ifndef NOEXERNLIBS

#include "glad/glad.h"
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <glm/glm.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#endif

//Position handled by Mesh?

class Mesh: public Object{
  public:
    Mesh(aiMesh *mesh, aiMaterial *mat, const char *modelName, glm::vec3 *scale);

    Mesh();

    virtual ~Mesh();

    void update();

    Image *getImage();
  protected:
    uint32_t VAO = 0;
    uint32_t VBO = 0;
    uint32_t EBO = 0;
    Image *oTexture = nullptr;
    uint32_t oVertices_size, oIndicesSize;
    PhysicsMesh *oMesh;

};

#endif
