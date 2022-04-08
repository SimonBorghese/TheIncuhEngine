#ifndef BSPMESH_HPP
#define BSPMESH_HPP
#include <vector>
#include <string>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <Image.hpp>
#include <Physics/PhysicsMesh.hpp>
#include <CBSP/CBSP.h>
#include <BSPFace.hpp>
#include <math.h>

class BSPMesh: public Mesh
{
    public:
        BSPMesh(CBSP *bspFile, float scale, PhysicsCore *core, Shader *iShader);
        virtual ~BSPMesh();

        void update();
  private:
      std::vector<BSPFace*> bFaces;
      Shader *oShader;
      uint32_t oModelLoc;

};

#endif // BSPMESH_HPP
