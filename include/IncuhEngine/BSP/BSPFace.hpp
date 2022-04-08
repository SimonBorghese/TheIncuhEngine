#ifndef BSPFACE_HPP
#define BSPFACE_HPP
#include <vector>
#include <string>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <Image.hpp>
#include <Physics/PhysicsMesh.hpp>
#include <CBSP/CBSP.h>

class BSPFace
{
    public:
        BSPFace(CBSP *bspFile, uint32_t face, uint32_t VBO);
        virtual ~BSPFace();

        void update();

        Image *getImage(){return oTexture;}

    protected:

    private:
        uint32_t VAO, EBO;
        Image *oTexture;
        uint32_t oVertices_size, oIndicesSize;
        PhysicsMesh *oMesh;
};

#endif // BSPFACE_HPP
