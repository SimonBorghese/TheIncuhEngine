#include <Shader.hpp>

Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
    FILE *file = fopen(vertexFile, "r");
    int fLength = 0;
    fseek(file, 0, SEEK_END);
    fLength = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *vertexShaderSource = (char*) malloc(sizeof(char) * fLength + 1);
    fread(vertexShaderSource, 1, fLength, file);
    vertexShaderSource[fLength] = '\0';
    fclose(file);

    file = fopen(fragmentFile, "r");
    fseek(file, 0, SEEK_END);
    fLength = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *fragmentShaderSource = (char*) malloc(sizeof(char) * fLength + 1);
    fread(fragmentShaderSource, 1, fLength, file);
    fragmentShaderSource[fLength] = '\0';
    fclose(file);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    mainShader = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);


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

    free(vertexShaderSource);
    free(fragmentShaderSource);
}

Shader::Shader(const char *vertexFile, const char *geometryFile, const char *fragmentFile){
    FILE *file = fopen(vertexFile, "r");
    int fLength = 0;
    fseek(file, 0, SEEK_END);
    fLength = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *vertexShaderSource = (char*) malloc(sizeof(char) * fLength + 1);
    fread(vertexShaderSource, 1, fLength, file);
    vertexShaderSource[fLength] = '\0';
    fclose(file);

    file = fopen(geometryFile, "r");
    fseek(file, 0, SEEK_END);
    fLength = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *geometryShaderSource = (char*) malloc(sizeof(char) * fLength + 1);
    fread(geometryShaderSource, 1, fLength, file);
    geometryShaderSource[fLength] = '\0';
    fclose(file);

    file = fopen(fragmentFile, "r");
    fseek(file, 0, SEEK_END);
    fLength = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *fragmentShaderSource = (char*) malloc(sizeof(char) * fLength + 1);
    fread(fragmentShaderSource, 1, fLength, file);
    fragmentShaderSource[fLength] = '\0';
    fclose(file);

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  mainShader = glCreateProgram();

  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);


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

  free(vertexShaderSource);
  free(geometryShaderSource);
  free(fragmentShaderSource);

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
