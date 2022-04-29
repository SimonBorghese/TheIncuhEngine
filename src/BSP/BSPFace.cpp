#include "BSPFace.hpp"

BSPFace::BSPFace(CBSP *bspFile, uint32_t face, uint32_t VBO)
{
    std::vector<uint32_t> indices;


        for (int m = 0; m < bspFile->mFaces[face].nMesh; m++){
            indices.push_back(bspFile->mMeshes[bspFile->mFaces[face].mesh + m].offset + bspFile->mFaces[face].vertex);
        }

    oIndicesSize = indices.size() * sizeof(uint32_t);

    if (strcmp(bspFile->mTextures[bspFile->mFaces[face].texture].name, "") != 0){

        //mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
        std::string base_filename = bspFile->mTextures[bspFile->mFaces[face].texture].name;
        base_filename = base_filename.substr(base_filename.find_last_of("/\\") + 1);

        oTexture = new Image(fmt::format("maps/textures/{}.jpg", base_filename.c_str()).c_str(), GL_TEXTURE0, GL_RGB, 1);
    }
    else{
        oTexture = new Image("", GL_TEXTURE0, GL_RGB, 1);;
    }

    //oTexture = new Image("", GL_TEXTURE0, GL_RGB, 1);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, oIndicesSize, indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

BSPFace::~BSPFace()
{
    //dtor
}

void BSPFace::update(){
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, (oIndicesSize / sizeof(uint32_t)), GL_UNSIGNED_INT,0);
}
