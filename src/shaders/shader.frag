#version 330 core
out vec4 FragColor;
in vec3 color;
in vec2 TexCoords;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float visibility_of_face;

void main(){
   // FragColor = mix(texture(Texture1, TexCoords), texture(Texture2, TexCoords), visibility_of_face);
   FragColor = texture(Texture1, TexCoords);
}
