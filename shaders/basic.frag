#version 150 core
#define PI 3.1415926536

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D texCity;
uniform sampler2D texCD;
uniform float time;

void main() {
    bool isReflected = Texcoord.y > 0.5;

    vec2 rTexcoord = vec2(Texcoord.x + int(!isReflected) * (sin(Texcoord.y * 60.0 + time * 2.0) / 60.0), (Texcoord.y - int(isReflected)) * (1 - 2 * int(isReflected)));

    vec4 colCity = texture(texCity, rTexcoord);
    vec4 colCD = texture(texCD, Texcoord);

    outColor = colCD;

    //outColor = colCity * 1.0 - (int(!isReflected) * vec4(0.4, 0.4, 0.1, 0.0));

    //outColor = mix(colCity, colVis, sin(time));

    //outColor = texture(tex, Texcoord) * vec4(1.0 - Color, 1.0);
}