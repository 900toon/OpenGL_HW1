#version 400 core

layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
layout (location = 1 ) in vec3 aTempColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 worldPos;
out vec2 texCoord;
out vec3 normal;

out vec3 tempColor;
void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    tempColor = aTempColor;
}