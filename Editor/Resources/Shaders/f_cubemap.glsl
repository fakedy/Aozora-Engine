#version 450 core
layout (location = 0) out vec4 finalColor;
in vec3 TexCoords;

//out vec4 FragColor;

uniform samplerCube skybox;

void main(){


    finalColor = vec4(texture(skybox, TexCoords).rgb, 1.0);

}