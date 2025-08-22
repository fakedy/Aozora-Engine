#version 430 core

#extension GL_ARB_bindless_texture : enable      // ADD THIS
#extension GL_ARB_shader_draw_parameters : enable // Keep this for gl_DrawIDARB
#extension GL_ARB_gpu_shader_int64  : enable


layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gEmissive;
layout (location = 4) out vec4 gProperties;


in vec2 textureCoord;
in vec3 fragPos;
flat in uint drawID;

in mat3 TBN;




struct Material{
	vec4 albedo;
	float metallic;
	float roughness;
	float ao;
	vec4 emissive;
	vec3 normal;
};



Material usedMaterial;

// SSBO
struct ObjectData {
	mat4 model;
	uint64_t   diffuseTextureHandle;
	uint64_t   emissiveTextureHandle;
	uint64_t   aoTextureHandle;
	uint64_t   metallicTextureHandle;
	uint64_t   roughnessTextureHandle;
	uint64_t   normalTextureHandle;
	vec4 albedo;
	vec4 emissive;
	vec3 normal;
	float metallic;
	float roughness;
	float ao;

};

layout(std430, binding = 4) readonly buffer ObjectBuffer {
	ObjectData objects[];
};



void main() {

	// get the data using draw id
	ObjectData data = objects[drawID];

	
/*
	if(usedMaterial.albedo.a < 0.05f){
		discard;
	}
*/

	// if we have a diffusetexture it will return true and then we take the float of that value and it returns 1
	// Albedo
	vec4 diffuseSample = texture(sampler2D(data.diffuseTextureHandle), textureCoord).rgba;
	usedMaterial.albedo = mix(data.albedo, diffuseSample, float(data.diffuseTextureHandle != 0));

	// Metallic
	float metallicSample = texture(sampler2D(data.metallicTextureHandle), textureCoord).r;
	usedMaterial.metallic = mix(data.metallic, metallicSample, float(data.metallicTextureHandle != 0));

	// Roughness
	float roughnessSample = texture(sampler2D(data.roughnessTextureHandle), textureCoord).r;
	usedMaterial.roughness = mix(data.roughness, roughnessSample, float(data.roughnessTextureHandle != 0));

	// Ambient Occlusion
	float aoSample = texture(sampler2D(data.aoTextureHandle), textureCoord).r;
	usedMaterial.ao = mix(data.ao, aoSample, float(data.aoTextureHandle != 0));

	// Emissive
	vec4 emissiveSample = texture(sampler2D(data.emissiveTextureHandle), textureCoord).rgba;
	usedMaterial.emissive = mix(data.emissive, emissiveSample, float(data.emissiveTextureHandle != 0));

	// Normal
	vec3 vertexNormalTangent = vec3(0.0, 0.0, 1.0);
	vec3 normalMapSample = texture(sampler2D(data.normalTextureHandle), textureCoord).rgb * 2.0 - 1.0;
	vec3 tangentNormal = mix(vertexNormalTangent, normalMapSample, float(data.normalTextureHandle != 0));
	usedMaterial.normal = normalize(TBN * tangentNormal);

	
// dont do this unless textures are working

	if(usedMaterial.albedo.a < 0.05f){
		discard;
	}

	gPosition = fragPos;
	gNormal = normalize(usedMaterial.normal);
	gAlbedo =  usedMaterial.albedo;
	gEmissive = usedMaterial.emissive;
	gProperties = vec4(usedMaterial.metallic, usedMaterial.roughness, usedMaterial.ao, 1.0f);

}