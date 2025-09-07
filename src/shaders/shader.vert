#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;

out vec3 color;
out vec2 TexCoords;

uniform mat4 aTrans;

void main() {
   gl_Position = aTrans * vec4(aPos, 1.0);
   color = aColor;
   TexCoords = aTexCoords;
}
