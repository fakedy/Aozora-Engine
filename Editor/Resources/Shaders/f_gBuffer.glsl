#version 430 core

#extension GL_ARB_bindless_texture : enable      
#extension GL_ARB_shader_draw_parameters : enable 
#extension GL_ARB_gpu_shader_int64  : enable


layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gEmissive;
layout (location = 4) out vec4 gProperties;


in vec2 textureCoord;
in vec3 fragPos;
flat in uint drawID;

in mat3 TBN;
in vec3 meshNormal;





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
	uint64_t   opacityTextureHandle;
	uint64_t   normalTextureHandle;
	vec4 albedo;
	vec4 emissive;
	vec4 normal;
	float metallic;
	float roughness;
	float ao;
	float pad2;

};

layout(std430, binding = 4) readonly buffer ObjectBuffer {
	ObjectData objects[];
};



void main() {

	// get the data using draw id
	ObjectData data = objects[drawID];

	



	// if we have a diffusetexture it will return true and then we take the float of that value and it returns 1
	// Albedo
	vec4 diffuseSample = vec4(1.0);
	if(data.diffuseTextureHandle != 0){
		diffuseSample = texture(sampler2D(data.diffuseTextureHandle), textureCoord).rgba;
	}
	usedMaterial.albedo = diffuseSample * data.albedo;

	if(data.opacityTextureHandle != 0){
		float opacity = texture(sampler2D(data.diffuseTextureHandle), textureCoord).r;
		if(opacity < 0.99){
			discard;
		}
	}
	if(usedMaterial.albedo.a < 0.9){
		discard;
	}
	// Metallic
	float metallicSample = 1.0;
	if(data.metallicTextureHandle != 0){
		metallicSample = texture(sampler2D(data.metallicTextureHandle), textureCoord).b;
	}

	usedMaterial.metallic = metallicSample * data.metallic;

	// Roughness
	float roughnessSample = 1.0;
	if(data.roughnessTextureHandle != 0){
		roughnessSample = texture(sampler2D(data.roughnessTextureHandle), textureCoord).g;
	}
	usedMaterial.roughness = roughnessSample * data.roughness;

	// Ambient Occlusion
	float aoSample = 1.0;
	if(data.aoTextureHandle != 0){	
		aoSample = texture(sampler2D(data.aoTextureHandle), textureCoord).r;
	}
	usedMaterial.ao = aoSample * data.ao;

	// Emissive
	vec4 emissiveSample = vec4(1.0);
	if(data.emissiveTextureHandle != 0){
		 emissiveSample = texture(sampler2D(data.emissiveTextureHandle), textureCoord).rgba;
	}
	usedMaterial.emissive = emissiveSample * data.emissive;

	// Normal
	vec3 tangentNormal = vec3(0.0, 0.0, 1.0); // Default flat normal
	if(data.normalTextureHandle != 0) {
    	tangentNormal = texture(sampler2D(data.normalTextureHandle), textureCoord).rgb * 2.0 - 1.0;
	}
	usedMaterial.normal = normalize(TBN * tangentNormal);


	gPosition = fragPos;
	gNormal = vec4(usedMaterial.normal* 0.5 + 0.5, 1.0);
	gAlbedo =  usedMaterial.albedo;
	gEmissive = usedMaterial.emissive;
	gProperties = vec4(usedMaterial.ao, usedMaterial.roughness, usedMaterial.metallic, 1.0f);

}