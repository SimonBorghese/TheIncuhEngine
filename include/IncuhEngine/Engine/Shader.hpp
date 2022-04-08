#ifndef SHADER_HPP
#define SHADER_HPP
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Image.hpp"


class Shader
{
    public:
        Shader(const char *vertexFile, const char *fragmentFile);
        Shader(const char *vertexFile, const char *geometryFile, const char *fragmentFile);
        virtual ~Shader();

        void useMain();

        GLint getUniformLocation(const char *name);

        int getInt(GLint location);

        void setMatrix4f(GLint location, const glm::mat4 &mat);
        void setInt(GLint location, int newInt);

        void bindTexture(Image *img, const char *unfi_name);

        void setVec3(GLint location, glm::vec3 newVec);
        
        void setFloat(GLint location, float newFloat);
    protected:
      unsigned int mainShader;
    private:
        unsigned int vertexShader, fragmentShader, geometryShader, computeShader;
};

#endif // SHADER_HPP
