#define STB_IMAGE_IMPLEMENTATION
#include <Image.hpp>

Image::Image(const char *fileName, GLenum texture_num, GLenum RGB_TYPE, int flip){
  glGenTextures(1, &texture);
  glActiveTexture(texture_num);
  glBindTexture(GL_TEXTURE_2D, texture);


  stbi_set_flip_vertically_on_load(flip);
  data = stbi_load(fileName, &width, &height, &nrChannels, 3);
  if (!data){
    if (strcmp("", fileName)){
      incuh_warning(fmt::format("Failed to load image: {}\n", fileName));
    }
    data = stbi_load("data/missing.png", &width, &height, &nrChannels, 3);
    if (!data){
      incuh_error("CRITICAL ERROR: UN-ABLE TO LOAD ESSENTIAL FILE\n");
    }
  }
  glTexImage2D(GL_TEXTURE_2D, 0, RGB_TYPE, width, height, 0, RGB_TYPE, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
  image_count = texture_num;
}
Image::Image() {}

Image::~Image(){

}

GLenum Image::getImageNum(){
  return image_count;
}

unsigned int Image::getTexture(){
  return texture;
}
