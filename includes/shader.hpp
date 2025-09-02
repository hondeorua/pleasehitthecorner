#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

class Shader{
public:
   unsigned int shaderID;

   Shader(const char* vertexPath, const char* fragmentPath);
   void use();

   // utilities
   void setBool(const char* name, bool value) const;
   void setInt(const char* name, int value) const;
   void setFloat(const char* name, float value) const;
};

#endif
