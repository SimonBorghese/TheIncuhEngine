#include "CubeMap.hpp"

CubeMap::CubeMap(const char *fileName, GLenum texture_num, GLenum RGB_TYPE, int flip, Shader *shader, glm::mat4 *view, glm::mat4 *projection) : cShader(shader), cView(view), cProjection(projection)
{
    glGenTextures(1, &texture);
    glActiveTexture(texture_num);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);


    stbi_set_flip_vertically_on_load(flip);
    for(int i = 0; i < 6; i++)
    {
        data = stbi_load(fmt::format("data/cubemap/{}/{}.jpg", fileName, i).c_str(), &width, &height, &nrChannels, 0);
        if (!data){
            if (strcmp("", fileName)){
                incuh_warning(fmt::format("Failed to load image: {}/{}.jpg\n", fileName, i));
            }
            data = stbi_load("data/missing.png", &width, &height, &nrChannels, 3);
            if (!data){
                incuh_error("CRITICAL ERROR: UN-ABLE TO LOAD ESSENTIAL FILE\n");
            }
        }
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, RGB_TYPE, width, height, 0, RGB_TYPE, GL_UNSIGNED_BYTE, data
        );
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //glGenerateMipmap(GL_TEXTURE_2D);
    image_count = texture_num;

    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f, // bottom right
         1.0f,  1.0f, -1.0f, // top right
         1.0f,  1.0f,  1.0f, // top left
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    //for (long unsigned int x = 0; x < (sizeof(skyboxVertices)/sizeof(float)); x++){
    //    skyboxVertices[x] *= 900.0f;
    //}

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
}

CubeMap::~CubeMap()
{
    //dtor
}

void CubeMap::update(){
    glDepthFunc(GL_LEQUAL);
    cShader->useMain();
    cShader->setMatrix4f(cShader->getUniformLocation("view"), glm::mat4(glm::mat3(*cView)));
    cShader->setMatrix4f(cShader->getUniformLocation("projection"), *cProjection);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

}
