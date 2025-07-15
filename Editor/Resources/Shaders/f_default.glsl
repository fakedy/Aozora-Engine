#version 450 core

out vec4 fragColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;


uniform sampler2D texture_diffuse;
uniform sampler2D texture_emissive;
uniform sampler2D texture_ao;

uniform sampler2D texture_metallic;
uniform sampler2D texture_roughness;
uniform sampler2D texture_normal;

// should be 0(false) by default
uniform bool has_texture_diffuse;
uniform bool has_texture_emissive;
uniform bool has_texture_ao;
uniform bool has_texture_metallic;
uniform bool has_texture_roughness;
uniform bool has_texture_normal;

uniform vec3 cameraPos;

struct Material{
	vec4 albedo;
	float metallic;
	float roughness;
	float ao;
	vec4 emissive;
	vec3 normal;
};

// temporary, might move this to an UBO/SSBO
uniform vec4 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform vec4 emissive;

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



	if(has_texture_diffuse){
		usedMaterial.albedo = texture(texture_diffuse, textureCoord).rgba;
	} else {
		usedMaterial.albedo = albedo;
	}

	if( has_texture_metallic){
		usedMaterial.metallic = texture(texture_metallic, textureCoord).b;
	} else {
		usedMaterial.metallic = metallic;
	}


	if( has_texture_roughness){
		usedMaterial.roughness = texture(texture_roughness, textureCoord).g; // or g i dont know, depends on texture
	} else {
		usedMaterial.roughness = roughness;
	}


	if( has_texture_ao){
		usedMaterial.ao = texture(texture_ao, textureCoord).r;
	} else {
		usedMaterial.ao = ao;
	}


	if( has_texture_emissive){
		usedMaterial.emissive = texture(texture_emissive, textureCoord).rgba;
	} else {
		usedMaterial.emissive = emissive;
	}


	if (has_texture_normal) {
    	vec3 tangentNormal = texture(texture_normal, textureCoord).rgb * 2.0 - 1.0;
    	usedMaterial.normal = normalize(tangentNormal); 
	} else {
		usedMaterial.normal = normalize(normal);
	}

	if(usedMaterial.albedo.a < 0.05f){
		discard;
	}


	// usedMaterial.normal = normal;

	float intensity = 2.0f;
	vec3 lightPos = vec3(0.8, 1.0, 0.3);
	vec3 lightColor = vec3(1.0, 0.98, 0.92) * intensity; ;
	vec3 lightDir = normalize(lightPos);

	float lightFactor = max(dot(usedMaterial.normal, lightDir), 0.0);


	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 h = normalize(lightDir + viewDir);
	vec3 f_0 = mix(vec3(0.04f), usedMaterial.albedo.rgb, usedMaterial.metallic);
	float d = ndf(usedMaterial.normal, h, usedMaterial.roughness);
	vec3 f = f(h, viewDir, f_0); // fresnelSchlick
	float g = g(usedMaterial.normal, viewDir, usedMaterial.roughness) * g(usedMaterial.normal, lightDir, usedMaterial.roughness); // GeometrySmith

	vec3 dfg = d*f*g;
	float NdotV = max(dot(viewDir, usedMaterial.normal), 0.0);
	float NdotL = max(dot(lightDir, usedMaterial.normal), 0.0);
	vec3 specular = dfg / ((4.0 * NdotV * NdotL) + 0.0001); // Cook-Torrance BRDF equation, specular

	vec3 k_s = f; 
	vec3 k_d = (vec3(1.0f) - k_s) * (1.0 - usedMaterial.metallic);

	vec3 diffuse = k_d * usedMaterial.albedo.rgb / pi; // reflective distribution function
	vec3 radiance = (diffuse + specular) * lightFactor * lightColor;


	vec3 emission = usedMaterial.emissive.rgb;
	vec3 ambient = vec3(0.3f) * usedMaterial.albedo.rgb * usedMaterial.ao;
	vec3 color = ambient + radiance + emission;

	fragColor = vec4(color, usedMaterial.albedo.a);

}