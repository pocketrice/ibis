#version 150 core

in vec2 Texcoord;
out vec4 outColor;

uniform sampler2D texBuffer;

void main() {
    outColor = texture(texBuffer, Texcoord);
}