#version 450 core

layout (location = 0) out vec4 finalColor;

in vec2 textureCoord;


uniform sampler2D colorTexture;


vec3 ACESFilm(vec3 x) {
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e), 0.0, 1.0);
}


void main(){

    vec3 color = texture(colorTexture, textureCoord).rgb;
    const float exposure = 1;
    vec3 exposedColor = color * exposure;
    vec3 ldrColor = ACESFilm(exposedColor);

    const float gamma = 2.2;
    ldrColor = pow(ldrColor, vec3(1.0 / gamma));

	finalColor = vec4(ldrColor, 1.0);
}