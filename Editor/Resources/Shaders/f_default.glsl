#version 330 core

out vec4 fragColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_emissive1;
uniform sampler2D texture_ao1;

uniform sampler2D texture_metallic1;
uniform sampler2D texture_roughness1;
uniform sampler2D texture_normal1;

const float PI = 3.14159265359;


vec3 fresnelSchlick(float cosTheta, vec3 F0){ // calculate ratio between specular and diffuse reflection
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float distributionGGX(vec3 norm, vec3 h, float roughness){
	float a = roughness*roughness;
	float a2 = a*a;
	float normDotH = max(dot(norm,h), 0.0);
	float NdotH2 = normDotH*normDotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float normdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = normdotV;
    float denom = normdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}


void main() {

	vec3 albedo = texture(texture_diffuse1, textureCoord).rgb;
	float metallic = texture(texture_metallic1, textureCoord).b;
	float roughness = texture(texture_roughness1, textureCoord).g;
	float ao = texture(texture_ao1, textureCoord).r;
	vec3 emissive = texture(texture_emissive1, textureCoord).rgb;

	vec3 lightPos = vec3(20, 10, 10);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightDir = normalize(lightPos - fragPos);

	vec3 norm = normalize(normal);
	vec3 v = normalize(vec3(0,0,0) - fragPos); // view vector
	vec3 h = normalize(v + lightDir); // halfway vector
	float distance = length(lightPos - fragPos);
	float attenuation = 1.0 / (distance*distance);
	vec3 radiance = lightColor;

	// calculate Cook-Torrance specular BRDF term
	vec3 F0 = vec3(0.04); // surface reflection at zero incidence
	F0 = mix(F0, albedo, metallic);
	vec3 F = fresnelSchlick(max(dot(h,v), 0.0), F0);

	// Calculate NDF
	float NDF = distributionGGX(norm, h, roughness);
	// Calculate G
	float G = GeometrySmith(norm, v, lightDir, roughness);

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(norm, v), 0.0) * max(dot(norm, lightDir), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	vec3 kS = F; // energy reflected
	vec3 kD = vec3(1.0) - kS; // refracted light
	kD *= 1.0 - metallic;
	
	float normDotL = max(dot(norm, lightDir), 0.0);
	vec3 Lo = vec3(0.0);
	Lo += (kD * albedo / PI + specular) * radiance * normDotL;

	vec3 ambient = 0.03 * albedo * ao;
	vec3 color = ambient + Lo + emissive;
	//color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));

	fragColor = vec4(color, 1.0);

}