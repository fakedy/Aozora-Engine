#version 430 core


#extension GL_ARB_bindless_texture : enable      // ADD THIS
#extension GL_ARB_shader_draw_parameters : enable // Keep this for gl_DrawIDARB
#extension GL_ARB_gpu_shader_int64  : enable

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aTextureCoord;



out vec2 textureCoord;
out vec3 fragPos;
out mat3 TBN;
flat out uint drawID;


uniform mat4 view;
uniform mat4 proj;

// SSBO
struct ObjectData {
	mat4 model;
	uint64_t  diffuseTextureHandle;
	uint64_t  emissiveTextureHandle;
	uint64_t  aoTextureHandle;
	uint64_t  metallicTextureHandle;
	uint64_t  roughnessTextureHandle;
	uint64_t  normalTextureHandle;
	vec4 albedo;
	vec4 emissive;
	vec3 normal;
	float metallic;
	float roughness;
	float ao;
};

layout(std430, binding = 4) buffer ObjectBuffer {
	ObjectData objects[];
};


void main() {

	// get the data using draw id
	ObjectData data = objects[gl_DrawIDARB];
	drawID = gl_DrawIDARB;
	textureCoord = aTextureCoord;
	fragPos = vec3(data.model * vec4(aPos, 1.0));

	// temp
	mat3 normalMatrix = mat3(transpose(inverse(data.model)));
	vec3 N = normalize(normalMatrix * aNormal);
    vec3 T = normalize(normalMatrix * aTangent);
    T = normalize(T - dot(T, N) * N); 
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);
	gl_Position = proj * view * vec4(fragPos, 1.0);

}