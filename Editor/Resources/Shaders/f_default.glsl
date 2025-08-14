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

// --- PBR HELPER FUNCTIONS ---

// D: Normal Distribution Function (Trowbridge-Reitz GGX)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

// G: Geometry Function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

// F: Fresnel-Schlick Approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}


void main() 
{
    // --- 1. UNPACK G-BUFFER DATA ---
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

    // --- 2. SETUP CORE VECTORS AND F0 ---
    vec3 V = normalize(cameraPos - fragPos);
    vec3 F0 = mix(vec3(0.04), albedo.rgb, metallic);

    // --- 3. CALCULATE DIRECT LIGHTING ---
    vec3 Lo = vec3(0.0); 
    for(int i = 0; i < activeLights; ++i)
    {
        vec3 L = normalize(lights[i].position - fragPos);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].color * lights[i].power * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(normal, H, roughness);
        float G = GeometrySmith(normal, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(normal, V), 0.0) * max(dot(normal, L), 0.0) + 0.001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

        float NdotL = max(dot(normal, L), 0.0);
        Lo += (kD * albedo.rgb / PI + specular) * radiance * NdotL;
    }

    // --- 4. CALCULATE AMBIENT LIGHTING ---
    vec3 F_ambient = fresnelSchlick(max(dot(normal, V), 0.0), F0);
    vec3 kS_ambient = F_ambient;
    vec3 kD_ambient = (vec3(1.0) - kS_ambient) * (1.0 - metallic);

    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 diffuse_ambient = irradiance * albedo.rgb;
    
    vec3 ambient = kD_ambient * diffuse_ambient; // * ao;

    // --- 5. FINAL COLOR ---
    // Combine all lighting components. The result is linear, HDR color.
    vec3 color = ambient + Lo + emissive;
    finalColor = vec4(color, albedo.a);
}