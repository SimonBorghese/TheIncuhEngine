#ifndef IMAGE_H
#define IMAGE_H
#include <Server/Logging.h>
#include <stb_image.h>
#include "glad/glad.h"
#include <stdio.h>

class Image{
  public:
    Image(const char *fileName, GLenum texture_num, GLenum RGB_TYPE, int flip);

    Image();

    virtual ~Image();

    GLenum getImageNum();
    unsigned int getTexture();

  protected:
    unsigned int texture;
    int width, height, nrChannels;
    unsigned char *data;
    GLenum image_count;

};

#endif
