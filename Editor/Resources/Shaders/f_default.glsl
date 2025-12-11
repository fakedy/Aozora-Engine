#version 450 core

// remake this entire file


layout (location = 0) out vec4 finalColor;

in vec2 textureCoord;

// --- UNIFORMS & STRUCTS ---
uniform vec3 cameraPos;
layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;
layout(binding = 3) uniform sampler2D gEmissive;
layout(binding = 4) uniform sampler2D gProperties;
layout(binding = 6) uniform samplerCube irradianceMap;
layout(binding = 7) uniform samplerCube skybox;

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

    vec3 fragPos;
    vec3 normal;
    vec4 albedo;
    vec3 emissive; 
    float metallic;
    float roughness;
    float ao;



vec3 calcIndirectLighting(){

    // diffuse part of indirect lighting
    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 diffuse_term = albedo.rgb * irradiance;


    // specular part of indirect lighting
    vec3 wo = normalize(cameraPos - fragPos);
    vec3 wi = normalize(reflect(-wo, normal));
    vec3 wh = normal; // or normalize(wi + wo)

    vec3 R = reflect(-wo, normal);
    // quick hack for testing, should use Importance Sampling Convolution.
    float lod = roughness * 5.0; // Assume 5 mip levels
    vec3 Li = textureLod(skybox, R, lod).rgb;


    // if plastic fresnel is 0.04, otherwise mixed by metallic
    vec3 fresnel = vec3(0.04);
    fresnel = mix(fresnel, albedo.rgb, metallic);


    vec3 F = fresnel + (1.0 - fresnel) * pow(1.0 - dot(wo,wh), 5.0);

    vec3 dialectric_term = F*Li + (1 - F) * diffuse_term;
    vec3 metal_term = F * Li;
    return metallic * metal_term + (1.0 - metallic) * dialectric_term;
}




void main(){

    fragPos = texture(gPosition, textureCoord).rgb;
    normal = normalize(texture(gNormal, textureCoord).rgb * 2.0 - 1.0);
    albedo = texture(gAlbedo, textureCoord);
    emissive = texture(gEmissive, textureCoord).rgb;
    metallic = texture(gProperties, textureCoord).b;
    roughness = texture(gProperties, textureCoord).g;
    ao = texture(gProperties, textureCoord).r;

    if(albedo.a < 0.05f) {
        discard;
    }


    vec3 light = calcIndirectLighting() + emissive;
    
    vec3 testcolor = texture(skybox, normal).rgb;

    finalColor = vec4(light, albedo.a);
}