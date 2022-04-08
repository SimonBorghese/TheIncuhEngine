#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP
#include <Image.hpp>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <fmt/format.h>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <glm/glm.hpp>

class CubeMap: public Image, public Mesh
{
    public:
        CubeMap(const char *fileName, GLenum texture_num, GLenum RGB_TYPE, int flip, Shader *shader, glm::mat4 *view, glm::mat4 *projection);
        virtual ~CubeMap();

        void update();

    protected:

    private:
        Shader *cShader;
        glm::mat4 *cView, *cProjection;
};

#endif // CUBEMAP_HPP
