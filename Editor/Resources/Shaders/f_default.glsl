#version 450 core

layout (location = 0) out vec4 finalColor;

in vec2 textureCoord;

uniform vec3 cameraPos;

struct Material{
	vec4 albedo;
	float metallic;
	float roughness;
	float ao;
	float emissive;
	vec3 normal;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gProperties;

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


	vec3 fragPos = texture(gPosition, textureCoord).rgb;

	usedMaterial.albedo = texture(gAlbedo, textureCoord).rgba;

	usedMaterial.metallic = texture(gProperties, textureCoord).r;

	usedMaterial.roughness = texture(gProperties, textureCoord).g; // or g i dont know, depends on texture

	usedMaterial.ao = texture(gProperties, textureCoord).b;

	usedMaterial.emissive = texture(gProperties, textureCoord).a;

	usedMaterial.normal = normalize(texture(gNormal, textureCoord).rgb);

	if(usedMaterial.albedo.a < 0.05f){
		discard;
	}


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


	float emission = usedMaterial.emissive;
	vec3 ambient = vec3(0.3f) * usedMaterial.albedo.rgb * usedMaterial.ao;
	vec3 color = ambient + radiance + (emission*usedMaterial.albedo.rgb); // idk if this is legit

	finalColor = vec4(color, usedMaterial.albedo.a);

}