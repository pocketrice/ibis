#version 150 core
#define PI 3.1415926536

in vec3 Color;
in vec2 Texcoord;

out vec4 outColor;

uniform sampler2D tex;
uniform float time;

void main() {
    vec4 texColor = texture(tex, Texcoord);

    if (texColor.a < 0.5) {
        discard;
    }

    outColor = texColor * vec4(Color, 1.0);
}