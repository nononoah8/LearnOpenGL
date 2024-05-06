#include "Shader.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "renderer.h"

Shader::Shader(const std::string& vertex_filepath, const std::string& fragment_filepath): m_vertex_FilePath(vertex_filepath), m_fragment_FilePath(fragment_filepath), m_RendererId(0) {
  ShaderProgramSource source = ParseShader(vertex_filepath, fragment_filepath);
  m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader(){
  GLCall(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const{
  GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const{
  GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3){
  GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

unsigned int Shader::GetUniformLocation(const std::string& name){
  if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end()){
    return m_UniformLocationCache[name];
  }
  if(m_UniformLocationCache[name] == -1){
    std::cout << "WARNING: uniform " << name << "doesn't exist";
  }
  unsigned int location = glGetUniformLocation(m_RendererId, name.c_str());

  m_UniformLocationCache[name] = location;
  return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& vertex_file, const std::string& fragment_file){
  std::ifstream vert_stream(vertex_file);
  std::string line1;
  std::stringstream vert_shader;

  while(getline(vert_stream, line1)){
    vert_shader << line1 << '\n';
  }
  
  std::ifstream frag_stream(fragment_file);
  std::string line2;
  std::stringstream frag_shader;

  while(getline(frag_stream, line2)){
    frag_shader << line2 << '\n';
  }

  return { vert_shader.str(), frag_shader.str() };
}

/*
Function to Compile a shader that takes in the type of shader and the source name as a string.
Also source string MUST be alive.
If you have a function that returns a string, should make it an l-value.
*/
unsigned int Shader::CompileShader(unsigned int type, const std::string& source){
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  
  /*
    glShaderSource sets the source code in shader to the source code in the array of strings specified by string. 
    This function takes in the id or the shader type, then the number of shaders, then a double pointer to the source
    It also takes in a double pointer to length which specifies an array of string lengths.
  */
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  GLint success;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &success));
  if(!success) {
    GLint length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char));
    
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << std::endl;
    std::cout << message << std::endl;

    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

/*
Creates the  shader using the CompileShader function.
*/
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader){
  //* Need to provide shader source code w our shader text and need to return something for that shader so we can bind it and then use it.
  //* Need to create our program
  unsigned int program = glCreateProgram();
  //* Create vertex shader
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  //* Create fragment shader
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  //* We now need to attach these 2 shaders into our program.
  //* Can think of it like having 2 different files and attaching them to our main program.
  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  //* Since the shaders have been attached to the program, we don't need the memory overhead anymore and can just delete them.
  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  //* Return the program.
  return program;
}