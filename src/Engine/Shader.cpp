#include <Shader.hpp>

Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    std::ifstream *shaderV = new std::ifstream(vertexFile, std::ios::binary);
    std::ifstream *shaderF = new std::ifstream(fragmentFile, std::ios::binary);

    std::string *shaderV_str = new std::string;
    shaderV_str->assign( (std::istreambuf_iterator<char>(*shaderV) ), (std::istreambuf_iterator<char>()    ) );
    std::string *shaderF_str = new std::string;
    shaderF_str->assign( (std::istreambuf_iterator<char>(*shaderF) ), (std::istreambuf_iterator<char>()    ) );

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    mainShader = glCreateProgram();

    const char *trueVSource = shaderV_str->append("\0").c_str();
    const char *trueFSource = shaderF_str->append("\0").c_str();

    glShaderSource(vertexShader, 1, &trueVSource, NULL);
    glShaderSource(fragmentShader, 1, &trueFSource, NULL);


    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex Shader Error: %s\n", infoLog);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment Shader Error: %s\n", infoLog);
    }

    glAttachShader(mainShader, vertexShader);
    glAttachShader(mainShader, fragmentShader);

    glLinkProgram(mainShader);

    glGetProgramiv(mainShader, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(mainShader, 512, NULL, infoLog);
        printf("Main Error: %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    delete trueVSource;
    delete trueFSource;
    delete (shaderF);
    delete (shaderV);
}

Shader::Shader(const char *vertexFile, const char *geometryFile, const char *fragmentFile){
  std::ifstream *shaderV = new std::ifstream(vertexFile, std::ios::binary);
  std::ifstream *shaderG = new std::ifstream(geometryFile, std::ios::binary);
  std::ifstream *shaderF = new std::ifstream(fragmentFile, std::ios::binary);

  std::string *shaderV_str = new std::string;
  shaderV_str->assign( (std::istreambuf_iterator<char>(*shaderV) ), (std::istreambuf_iterator<char>()    ) );

  std::string *shaderG_str = new std::string;
  shaderG_str->assign( (std::istreambuf_iterator<char>(*shaderG) ), (std::istreambuf_iterator<char>()    ) );

  std::string *shaderF_str = new std::string;
  shaderF_str->assign( (std::istreambuf_iterator<char>(*shaderF) ), (std::istreambuf_iterator<char>()    ) );

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  mainShader = glCreateProgram();

  const char *trueVSource = shaderV_str->append("\0").c_str();
  const char *trueGSource = shaderV_str->append("\0").c_str();
  const char *trueFSource = shaderF_str->append("\0").c_str();

  glShaderSource(vertexShader, 1, &trueVSource, NULL);
  glShaderSource(geometryShader, 1, &trueGSource, NULL);
  glShaderSource(fragmentShader, 1, &trueFSource, NULL);


  glCompileShader(vertexShader);
  glCompileShader(geometryShader);
  glCompileShader(fragmentShader);

  int success;
  char infoLog[512];

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("Vertex Shader Error: %s\n", infoLog);
  }

  glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
    printf("Geometry Shader Error: %s\n", infoLog);
  }

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("Fragment Shader Error: %s\n", infoLog);
  }

  glAttachShader(mainShader, vertexShader);
  glAttachShader(mainShader, geometryShader);
  glAttachShader(mainShader, fragmentShader);

  glLinkProgram(mainShader);

  glGetProgramiv(mainShader, GL_LINK_STATUS, &success);
  if (!success){
    glGetProgramInfoLog(mainShader, 512, NULL, infoLog);
    printf("Main Error: %s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);

  delete trueVSource;
  delete trueGSource;
  delete trueFSource;
  delete (shaderF);
  delete (shaderG);
  delete (shaderV);

}


Shader::~Shader()
{
    glDeleteProgram(mainShader);
}

void Shader::useMain(){
    glUseProgram(mainShader);
}

GLint Shader::getUniformLocation(const char *name){
  return glGetUniformLocation(mainShader, name);
}
void Shader::setMatrix4f(GLint location, const glm::mat4 &mat){
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}


void Shader::bindTexture(Image *img, const char *unfi_name){
  glActiveTexture(img->getImageNum());
  glBindTexture(GL_TEXTURE_2D, img->getTexture());

  glUniform1i(glGetUniformLocation(mainShader, unfi_name), img->getImageNum() - GL_TEXTURE0);
}

int Shader::getInt(GLint location){
  int gotten_int;
  glGetUniformiv(mainShader, location, &gotten_int);
  return gotten_int;
}

void Shader::setInt(GLint location, int newInt){
  glUniform1i(location, newInt);
}

void Shader::setVec3(GLint location, glm::vec3 newVec){
  //glUniform3fv(location, 3, glm::value_ptr(newVec));
  glUniform3f(location, newVec.x, newVec.y, newVec.z);
}

void Shader::setFloat(GLint location, float newFloat){
  glUniform1f(location, newFloat);
}
