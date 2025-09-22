#version 400 core
out vec4 color;

in vec4 worldPos;
in vec2 texCoord;
in vec3 normal;

in vec3 tempColor;

void main(){
    color = vec4(tempColor, 1.0);
}