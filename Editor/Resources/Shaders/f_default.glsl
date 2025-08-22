#version 450 core

// remake this entire file


layout (location = 0) out vec4 finalColor;

in vec2 textureCoord;

// --- UNIFORMS & STRUCTS ---
uniform vec3 cameraPos;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gEmissive;
uniform sampler2D gProperties;
uniform samplerCube irradianceMap;

struct Light {
    vec3 position;
    vec3 color;
    float linear;
    float quadratic;
    float radius;
    float power;
};
const int maxLights = 32;
uniform Light lights[maxLights];
uniform int activeLights;


const float PI = 3.14159265359;


    // normal distribution function
float GGXNormalDistribution(float roughness, float NdotH){

    return 0;
}

    // Fresnel Schlick
vec3 F(float cosTheta, vec3 F0){

    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float G(){

    return 0;
}




void main(){

    vec3 fragPos = texture(gPosition, textureCoord).rgb;
    vec3 normal = normalize(texture(gNormal, textureCoord).rgb);
    vec4 albedo = texture(gAlbedo, textureCoord);
    vec3 emissive = texture(gEmissive, textureCoord).rgb;
    float metallic = texture(gProperties, textureCoord).r;
    float roughness = texture(gProperties, textureCoord).g;
    float ao = texture(gProperties, textureCoord).b;

    if(albedo.a < 0.05f) {
        discard;
    }

    vec3 viewV = normalize(cameraPos - fragPos);
    vec3 F0 = mix(vec3(0.04), albedo.rgb, metallic);

    vec3 diffuse = albedo.rgb;
    vec3 specular = vec3(0.0,0.0,1.0);

    // implement light contributions
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < activeLights; ++i){

        
    }

    vec3 F_ambient = F(max(dot(normal, viewV), 0.0), F0);
    vec3 kS_ambient = F_ambient;
    vec3 kD_ambient = (vec3(1.0) - kS_ambient) * (1.0 - metallic);

    // ambient light
    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 diffuse_ambient = irradiance * albedo.rgb;
    vec3 finalAmbient = diffuse_ambient * kD_ambient;

    vec3 color =  Lo + emissive + diffuse_ambient;
    finalColor = vec4(color, albedo.a);
}