#include <BSPMesh.hpp>
BSPMesh::BSPMesh(CBSP *bspFile, float scale, PhysicsCore *core, Shader *iShader) : oShader(iShader)
{
    std::vector<float> vertices;
    std::vector<uint32_t> indices;
    CBSP *bsp = bspFile;


    for (int v = 0; v < bspFile->nVertexs; v++){
        //bsp->mVertices
        vertices.push_back(round(bspFile->mVertices[(v)].pos[1] * scale));
        vertices.push_back(round(bspFile->mVertices[(v)].pos[2] * scale));
        vertices.push_back(round(bspFile->mVertices[(v)].pos[0] * scale));
        vertices.push_back(bspFile->mVertices[(v)].normal[0]);
        vertices.push_back(bspFile->mVertices[(v)].normal[0]);
        vertices.push_back(bspFile->mVertices[(v)].normal[0]);
        vertices.push_back(bspFile->mVertices[(v)].texcord[0][0] / 10.0f);
        vertices.push_back(bspFile->mVertices[(v)].texcord[0][1] / 10.0f);

        indices.push_back(v);
    }
    oVertices_size = vertices.size() * sizeof(float);

    /*
    if (mat != NULL){
        aiString tex_loc;

        mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
        std::string base_filename = tex_loc.C_Str();
        base_filename = base_filename.substr(base_filename.find_last_of("/\\") + 1);

        oTexture = new Image(fmt::format("data/{}/textures/{}", modelName, base_filename.c_str()).c_str(), GL_TEXTURE0, GL_RGB, 1);
    }
    else{
        oTexture = new Image("", GL_TEXTURE0, GL_RGB, 1);;
    }
    */
    oTexture = new Image("", GL_TEXTURE0, GL_RGB, 1);


    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, oVertices_size, vertices.data(), GL_STATIC_DRAW);

    for (int f = 0; f < bspFile->nFaces; f++){
        bFaces.push_back(new BSPFace(bsp, f, VBO));
    }


    oIndicesSize = indices.size() * sizeof(uint32_t);

    std::vector<float> *targetVertices;
    std::vector<PhysicsObject*> targetMeshes;
    for (int m = 0; m < bspFile->nFaces; m++){
            targetVertices = new std::vector<float>;

                for (int x = 0 ; x < bspFile->mFaces[m].nMesh; x++){
                        int targetoffset = bspFile->mMeshes[bspFile->mFaces[m].mesh + x].offset + bspFile->mFaces[m].vertex;
                        targetVertices->push_back((bspFile->mVertices[targetoffset].pos[1] * scale));
                        targetVertices->push_back((bspFile->mVertices[targetoffset].pos[2] * scale));
                        targetVertices->push_back((bspFile->mVertices[targetoffset].pos[0] * scale));

                }

                targetMeshes.push_back(new PhysicsMesh(physx::PxVec3(0.0f, 0.0f, 0.0f), targetVertices, STATIC, new PhysicsMaterial((0.2f), (0.4f), (0.0f), core), core ));
                delete targetVertices;

        }

        for (long unsigned int p = 0; p < targetMeshes.size(); p++){
            core->addPhysicsObject(targetMeshes.at(p));
        }

    oModelLoc = oShader->getUniformLocation("model");
}

BSPMesh::~BSPMesh()
{
    //dtor
}

void BSPMesh::update(){
    oShader->setMatrix4f(oModelLoc, glm::mat4(1.0f));
    for (long unsigned int b = 0; b < bFaces.size(); b++){
        oShader->bindTexture(bFaces.at(b)->getImage(), "diffuse_mat");
        bFaces.at(b)->update();
    }

}

