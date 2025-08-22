#version 450 core

layout (location = 0) out vec4 finalColor;

in vec2 textureCoord;


uniform sampler2D colorTexture;


void main(){

    float exposure = 1.0;
    vec3 color = texture(colorTexture, textureCoord).rgb;
	const float gamma = 2.2;
	vec3 mapped = vec3(1.0) - exp(-color * exposure);
	mapped = pow(mapped, vec3(1.0 / gamma));



	finalColor = vec4(mapped, 1.0);
}