#version 330 core

out vec4 fragColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 fragPos;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;


void main() {

	vec3 lightColor = vec3(1.0, 1.0, 1.0);


	vec3 ambient = 0.1 * lightColor;

	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(vec3(20, 10, 10) - fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	//vec4 normalColor = vec4(abs(norm.x), abs(norm.y), abs(norm.z), 1.0f);
	fragColor = vec4(ambient + diffuse, 1.0) *  texture(texture_diffuse1, textureCoord); // * color;
	//fragColor = texture(texture_diffuse1, textureCoord);

}