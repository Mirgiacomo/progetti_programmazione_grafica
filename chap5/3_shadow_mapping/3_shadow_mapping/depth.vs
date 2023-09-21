// prende un modello per oggetto, un vertice e trasforma tutti i vertici in uno spazio luminoso usando lightSpaceMatrix
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}