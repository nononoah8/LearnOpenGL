#include "renderer.h"

#include <iostream>

void GLClearError(){
  while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* fn, const char* file, int line){
  while(GLenum error = glGetError()){
    std::cout << "OpenGL error: " << error << " [" << fn << "] [" << file << "] [" << line << "]\n";
    return false;
  }
  return true;
}