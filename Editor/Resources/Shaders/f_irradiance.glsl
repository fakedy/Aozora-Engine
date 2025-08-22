#version 450 core
layout (location = 0) out vec4 finalColor;
in vec3 TexCoords;

const float PI = 3.14159265359;

uniform samplerCube environmentMap;

void main(){
    
    vec3 irradiance = vec3(0.0);

    int samples = 0;
    // coordinate system
    vec3 N = normalize(TexCoords);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up = cross(N, right);

    // lower value = more detailed
    float sampleDelta = 0.025;
    for(float phi = 0.0; phi < 2.0*PI; phi += sampleDelta){
        for(float theta = 0.0; theta < 0.5*PI; theta += sampleDelta){

            // Convert spherical coordinates to a sample vector in tangent space.
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            
            // Convert from tangent space to world space.
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 
            irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);;
            samples++;
        }
    }


    irradiance = PI * irradiance / samples;
    finalColor = vec4(irradiance, 1.0); // fake atm

}