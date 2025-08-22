#version 450 core

out vec4 fragColor;

in vec3 worldPos;

in float gridSize;

uniform vec2 screenSize;
uniform vec3 cameraPos;


float gridCellSize = 1.0;
float lineWidth = 4.0;
float minPixel = 2.0;
vec3 lineColor = vec3(0.8392, 0.8392, 0.8392);

float log10(float x){
    return (0.4342944819) * log(x);
} 
    

void main(){

    vec2 derivX = vec2(dFdx(worldPos.x), dFdy(worldPos.x));
    vec2 derivZ = vec2(dFdx(worldPos.z), dFdy(worldPos.z));

    float dmagX = length(derivX);
    float dmagZ = length(derivZ);

    vec2 dmagXZ = vec2(dmagX, dmagZ);
    float l = length(dmagXZ);



    vec2 test = (mod(worldPos.xz, gridCellSize ) / (dmagXZ * lineWidth));

    float lod = max(1.0 - abs(test.x*2.0 - 1.0), 1.0 - abs(test.y*2.0 - 1.0));


    float fallOff = (1.0 - (length(worldPos.xz - cameraPos.xz) / gridSize) );

    float alpha = lod* fallOff;


    fragColor = vec4(lineColor, alpha);

}