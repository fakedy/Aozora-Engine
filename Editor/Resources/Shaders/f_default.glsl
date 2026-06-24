#version 450 core


layout (location = 0) out vec4 finalColor;

in vec2 textureCoord;

uniform vec3 cameraPos;
uniform mat4 invView;
layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D gAlbedo;
layout(binding = 3) uniform sampler2D gEmissive;
layout(binding = 4) uniform sampler2D gProperties;
layout(binding = 6) uniform samplerCube irradianceMap;
layout(binding = 7) uniform samplerCube skybox;
layout(binding = 8) uniform samplerCube prefilterMap;

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float linear;
    float quadratic;
    float radius;
    float power;
    int type;
};

const int LIGHT_DIRECTIONAL = 0;
const int LIGHT_AREA = 1;
const int LIGHT_POINT = 2;
const int LIGHT_SPOT = 3;

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



    // Approximate relative surface area of microfacets aligned to halfway vector
float D(vec3 halfwayVector){ // Normal distribution function
    // Trowbridge-Reitz GGX
    float a2 = roughness*roughness;
    float nDotH = max(dot(normal, halfwayVector), 0.0);
    float nDotH2 = nDotH * nDotH;

    return  a2 / (PI * (nDotH2 * (a2 - 1.0) + 1) * (nDotH2 * (a2 - 1.0) + 1));
}

    // Ratio of light refracted vs reflected
vec3 F( vec3 h, vec3 wi, vec3 F0){ // Fresnel equation

    // Fresnel-Schlick
    return F0 + (1.0 - F0) * pow(1.0 - max(dot(h,wi), 0.0), 5.0);
}


float schlickGGX(vec3 l, float k){
    return max(dot(normal, l), 0.001) / (max(dot(normal, l), 0.001)*(1 - k) + k);
}
    // approximate relative surface area where micro surface details overshadow each other.
float G(vec3 wi, vec3 wo, float k){ // Geometry function, Smith's method
    // Schlick-GGX
    float ggx1 = schlickGGX(wi, k);
    float ggx2 = schlickGGX(wo, k);

    return ggx1 * ggx2;
}

vec3 BRDF(vec3 wi, vec3 wo, float k){

    // our halfway vector
    vec3 hVec = normalize(wi + wo);

    vec3 lambert = albedo.rgb / PI;
    float D = D(hVec);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo.rgb, metallic);
    vec3 F = F(hVec, wi, F0);
    float G = G(wi, wo, k);

    vec3 cookTorrance = D*F*G / (4*max(dot(wo, normal), 0.0) * max(dot(wi, normal), 0.0) + 0.001);

    vec3 Ks = F;
    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic); // conversion of energy and factoring in metallic

    // omit Ks, already factored into cookTorrance by F
    return Kd*lambert + cookTorrance;
}


vec3 calcIndirectLighting(){

    // diffuse part of indirect lighting
    vec3 irradiance = texture(irradianceMap, mat3(invView) * normal).rgb;
    vec3 viewDir = normalize(-fragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo.rgb, metallic);

    vec3 Ks = F(normal, viewDir, F0);
    vec3 Kd = (vec3(1.0) - Ks) * (1.0 - metallic);
    vec3 diffuse_term = (Kd * (albedo.rgb * irradiance));

    // specular part...



    return diffuse_term;

}


vec3 calculateDirectIllumination(){

    vec3 Lo = vec3(0,0,0);
    for(int i = 0; i < activeLights; i++){
        Light light = lights[i];

        switch(light.type){
            case LIGHT_DIRECTIONAL:
                break;
            case LIGHT_AREA:
                break;
            case LIGHT_POINT:
                break;
            case LIGHT_SPOT:
                break;
            default:
                break;
        }

        vec3 lightDir = normalize(-light.direction);
        vec3 lightColor = light.color;

        float lightPower = light.power; // lux
        vec3 lightIntensity = lightColor * lightPower;

        // viewspace direction
        vec3 viewDir = normalize(-fragPos);

        float k = (roughness + 1)*(roughness + 1) / 8.0;
        // following rendering equation from one light direction
        Lo += BRDF(lightDir, viewDir, k) * lightIntensity * max(dot(normal, lightDir), 0.0);
        
    }

    

    return Lo;

}




void main(){

    fragPos = texture(gPosition, textureCoord).rgb;
    normal = normalize(texture(gNormal, textureCoord).rgb * 2.0 - 1.0);
    albedo = texture(gAlbedo, textureCoord);
    emissive = texture(gEmissive, textureCoord).rgb;
    metallic = texture(gProperties, textureCoord).b;
    roughness = texture(gProperties, textureCoord).g;
    ao = texture(gProperties, textureCoord).r;


    vec3 reflectance = calculateDirectIllumination() + calcIndirectLighting() + emissive.rgb;
    //vec3 reflectance = calcIndirectLighting() + emissive.rgb;
    //vec3 reflectance = calculateDirectIllumination() + emissive.rgb;
    
    finalColor = vec4(reflectance, 1.0);
}