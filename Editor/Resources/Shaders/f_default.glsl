#version 330 core

out vec4 fragColor;
in vec3 textureCoords;
in vec3 normal;
in vec3 fragPos;

uniform sampler2DArray ourTexture;


void main() {

	vec3 lightColor = vec3(1.0, 1.0, 1.0);


	vec3 ambient = 0.3 * lightColor;
	vec3 lightDir = vec3(1.0, -1.0, 1.0);
	float diff = max(dot(-lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	//vec4 color = vec4(ambient + diffuse, 1.0)* texture(ourTexture, textureCoords);
	fragColor = vec4(abs(normal.x), abs(normal.y), abs(normal.z), 1.0f);
	//fragColor = vec4(ambient + diffuse, 1.0) * color;

}