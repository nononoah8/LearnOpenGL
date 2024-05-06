#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader {
public:
  Shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  // Set uniforms
  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
  ShaderProgramSource ParseShader(const std::string& vertex_file, const std::string& fragment_file);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
  unsigned int GetUniformLocation(const std::string& name);
  std::string m_vertex_FilePath;
  std::string m_fragment_FilePath;
  unsigned int m_RendererId;
  std::unordered_map<std::string, int> m_UniformLocationCache;
};