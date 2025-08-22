#version 450 core


out vec3 worldPos;
out float gridSize;


uniform mat4 view;
uniform mat4 proj;
uniform vec3 cameraPos;



const vec3 quad[4] = {
    vec3(-1.0, 0.0, -1.0),
    vec3(1.0, 0.0, -1.0),
    vec3(1.0, 0.0, 1.0),
    vec3(-1.0, 0.0, 1.0),
};


const int indices[6] = int[6](0,2,1,2,0,3);


void main(){

    gridSize = 100.0;
    int i = indices[gl_VertexID];
    vec4 pos = vec4(quad[i]*gridSize, 1.0);
    pos.x += cameraPos.x;
    pos.z += cameraPos.z;
    gl_Position = proj * view * pos;

    worldPos = pos.xyz;

}

