#version 450 core
layout(location = 0) in vec3 aPos;

layout(location = 3) in vec2 aTextureCoord;

out vec2 textureCoord;

void main() {

	textureCoord = aTextureCoord;
	gl_Position = vec4(aPos, 1.0);

}