#version 450 core

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

uniform bool has_texture_diffuse1 = false;
uniform bool has_texture_emissive1 = false;
uniform bool has_texture_ao1 = false;
uniform bool has_texture_metallic1 = false;
uniform bool has_texture_roughness1 = false;
uniform bool has_texture_normal1 = false;

uniform vec3 cameraPos;

struct Material{
	vec3 albedo;
	float metallic;
	float roughness;
	float ao;
	vec3 emissive;
	vec3 normal;
};

// temporary, might move this to an UBO/SSBO
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform vec3 emissive;

Material usedMaterial;

const float pi = 3.1415926535f;

float ndf(vec3 n, vec3 h, float roughness){

	float a = roughness*roughness;
	float a2 = a*a;

	float NdotH = max(dot(n,h), 0.0);
	float NdotH2 = NdotH * NdotH;

	float calc = ( (NdotH2) * (a2 - 1.0) + 1.0);

	float ndf = a2 / (pi * (calc*calc));

	return ndf;
}

vec3 f(vec3 h, vec3 v, vec3 f_0){ // fresnelSchlick

	vec3 f = f_0 + (1-f_0)*pow(clamp(1 - (dot(h,v)), 0.0f, 1.0f), 5.0f);

	return f;
}

float g(vec3 n, vec3 v, float roughness){

	float k = pow(roughness+1, 2.0f)/8.0f;

	float NdotV = max(dot(n,v), 0.0);

	float g = NdotV / (NdotV * (1.0 - k) + k);

	return g;
}

void main() {


	if(has_texture_diffuse1){
		usedMaterial.albedo = texture(texture_diffuse1, textureCoord).rgb;
	} else {
		usedMaterial.albedo = albedo;
	}

	if( has_texture_metallic1){
		usedMaterial.metallic = texture(texture_metallic1, textureCoord).b;
	} else {
		usedMaterial.metallic = metallic;
	}


	if( has_texture_roughness1){
		usedMaterial.roughness = texture(texture_roughness1, textureCoord).g; // or g i dont know
	} else {
		usedMaterial.roughness = roughness;
	}


	if( has_texture_ao1){
		usedMaterial.ao = texture(texture_ao1, textureCoord).r;
	} else {
		usedMaterial.ao = ao;
	}


	if( has_texture_emissive1){
		usedMaterial.emissive = texture(texture_emissive1, textureCoord).rgb;
	} else {
		usedMaterial.emissive = emissive;
	}


	if (has_texture_normal1) {
    	vec3 tangentNormal = texture(texture_normal1, textureCoord).rgb * 2.0 - 1.0;
    	usedMaterial.normal = normalize(tangentNormal); 
	} else {
		usedMaterial.normal = normal;
	}


	usedMaterial.normal = normal;

	float intensity = 2.0f;
	vec3 lightPos = vec3(0.8, 1.0, 0.3);
	vec3 lightColor = vec3(1.0, 0.98, 0.92) * intensity; ;
	vec3 lightDir = normalize(lightPos);

	float lightFactor = max(dot(usedMaterial.normal, lightDir), 0.0);


	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 h = normalize(lightDir + viewDir);
	vec3 f_0 = mix(vec3(0.04f), usedMaterial.albedo, usedMaterial.metallic);
	float d = ndf(usedMaterial.normal, h, usedMaterial.roughness);
	vec3 f = f(h, viewDir, f_0); // fresnelSchlick
	float g = g(usedMaterial.normal, viewDir, usedMaterial.roughness) * g(usedMaterial.normal, lightDir, usedMaterial.roughness); // GeometrySmith

	vec3 dfg = d*f*g;
	float NdotV = max(dot(viewDir, usedMaterial.normal), 0.0);
	float NdotL = max(dot(lightDir, usedMaterial.normal), 0.0);
	vec3 specular = dfg / ((4.0 * NdotV * NdotL) + 0.0001); // Cook-Torrance BRDF equation, specular

	vec3 k_s = f; 
	vec3 k_d = (vec3(1.0f) - k_s) * (1.0 - usedMaterial.metallic);

	vec3 diffuse = (k_d * usedMaterial.albedo / pi) + specular; // reflective distribution function

	vec3 light = diffuse * lightFactor * lightColor;


	vec3 emission = usedMaterial.emissive;

	vec3 ambient = vec3(0.3) * usedMaterial.albedo * usedMaterial.ao;
	vec3 color = ambient + light + emission;

	fragColor = vec4(color, 1.0);

}