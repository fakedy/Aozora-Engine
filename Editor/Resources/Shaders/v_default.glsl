#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextureCoord;

out vec2 textureCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main() {

	textureCoord = aTextureCoord;
	fragPos = vec3(model * vec4(aPos, 1.0));
	normal = normalize(mat3(transpose(inverse(model))) * aNormal);

	gl_Position = proj * view * vec4(fragPos, 1.0);

}