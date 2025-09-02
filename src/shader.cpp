#include "shader.hpp"

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::ifstream vert_shader;
  vert_shader.open(vertexPath);
  if (!(vert_shader.is_open())) {
    throw std::runtime_error("Couldn't read shader.vert");
  }
  std::string vert_shader_source;
  std::string line;
  while (std::getline(vert_shader, line)) {
    vert_shader_source += "\n" + line;
  }
  std::cout << "vert shader: " << std::endl << vert_shader_source << std::endl;
  vert_shader.close();

  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const char *source_vert = vert_shader_source.c_str();
  glShaderSource(vertex_shader, 1, &source_vert, NULL);
  glCompileShader(vertex_shader);
  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, GL_INFO_LOG_LENGTH, NULL, info_log);
    std::cerr << "Vertex shader not compiled gud\n" << info_log << std::endl;
    std::exit(EXIT_FAILURE);
  }

  std::ifstream frag_shader;
  frag_shader.open(fragmentPath);
  if (!(frag_shader.is_open())) {
    throw std::runtime_error("Couldn't read shader.vert");
  }
  std::string frag_shader_source;
  while (std::getline(frag_shader, line)) {
    frag_shader_source += "\n" + line;
  }
  std::cout << "frag shader: " << std::endl << frag_shader_source << std::endl;
  frag_shader.close();

  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const char *source_frag = frag_shader_source.c_str();
  glShaderSource(fragment_shader, 1, &source_frag, NULL);
  glCompileShader(fragment_shader);
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, GL_INFO_LOG_LENGTH, NULL, info_log);
    std::cerr << "fragment shader not compiled gud\n" << info_log << std::endl;
    std::exit(EXIT_FAILURE);
  }

  shaderID = glCreateProgram();

  glAttachShader(shaderID, vertex_shader);
  glAttachShader(shaderID, fragment_shader);
  glLinkProgram(shaderID);
  glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderID, GL_INFO_LOG_LENGTH, NULL, info_log);
    std::cerr << "shader_program not compiled gud\n" << info_log << std::endl;
    std::exit(EXIT_FAILURE);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
};

void Shader::use() { glUseProgram(shaderID); }

void Shader::setBool(const char *name, bool value) const {
  unsigned int uniformLocation = glGetUniformLocation(shaderID, name);
  glUniform1i(uniformLocation, value);
}
void Shader::setInt(const char *name, int value) const {
  unsigned int uniformLocation = glGetUniformLocation(shaderID, name);
  glUniform1i(uniformLocation, value);
}
void Shader::setFloat(const char *name, float value) const {
  unsigned int uniformLocation = glGetUniformLocation(shaderID, name);
  glUniform1f(uniformLocation, value);
}
