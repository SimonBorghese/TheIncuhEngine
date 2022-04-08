#include "glm/fwd.hpp"
#include <Mesh.hpp>
#include <cstdint>

Mesh::Mesh(aiMesh *mesh, aiMaterial *mat, const char *modelName, glm::vec3 *scale){
  std::vector<float> vertices;
  std::vector<uint32_t> indices;
  for (unsigned int v = 0; v < mesh->mNumVertices; v++){
    vertices.push_back(mesh->mVertices[v].x * scale->x);
    vertices.push_back(mesh->mVertices[v].y  * scale->y);
    vertices.push_back(mesh->mVertices[v].z   * scale->z);
    // 1 2 0
    // y z x
    if (mesh->HasNormals()){
      vertices.push_back(mesh->mNormals[v].x);
      vertices.push_back(mesh->mNormals[v].y);
      vertices.push_back(mesh->mNormals[v].z);
    }
    else{
      vertices.push_back(1.0f);
      vertices.push_back(1.0f);
      vertices.push_back(1.0f);
    }
    for (int t = 0; t < AI_MAX_NUMBER_OF_TEXTURECOORDS; t++){
        if (mesh->mTextureCoords[t]){
            vertices.push_back((&mesh->mTextureCoords[t][v])->x );
            vertices.push_back((&mesh->mTextureCoords[t][v])->y );
            break;
        }
    }
    if (!mesh->mTextureCoords[0]){
      vertices.push_back(0.0f);
      vertices.push_back(0.0f);
    }
  }

  // Next, we go though all the faces
  for (unsigned int f = 0; f < mesh->mNumFaces; f++){
    // Now we yoink their indices
    for (unsigned int i = 0; i < mesh->mFaces[f].mNumIndices; i++){
      indices.push_back(mesh->mFaces[f].mIndices[i]);
    }
  }

  oVertices_size = vertices.size() * sizeof(float);
  oIndicesSize = indices.size() * sizeof(uint32_t);
  if (mat != NULL){
    aiString tex_loc;

    mat->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
    std::string base_filename = tex_loc.C_Str();
    base_filename = base_filename.substr(base_filename.find_last_of("/\\") + 1);
    //Model Name unused for now, keep at end
    oTexture = new Image(fmt::format("data/textures/{}", base_filename.c_str(), modelName).c_str(), GL_TEXTURE0, GL_RGB, 1);
  }
  else{
    oTexture = new Image("", GL_TEXTURE0, GL_RGB, 1);;
  }

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, oIndicesSize, indices.data(), GL_STATIC_DRAW);
  glBufferData(GL_ARRAY_BUFFER, oVertices_size, vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3*sizeof(float)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
}

Mesh::Mesh(){}

Mesh::~Mesh(){

}

void Mesh::update(){
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, (oIndicesSize / sizeof(uint32_t)), GL_UNSIGNED_INT,0);
}

Image *Mesh::getImage(){return oTexture;}
