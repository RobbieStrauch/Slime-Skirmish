#version 430

// Modified from: https://www.shadertoy.com/view/3sGGRz

layout (location = 0) in vec2 inUV;
layout (location = 0) out vec3 outColor;

uniform layout (binding = 0) sampler2D normals;

uniform float u_Strength;
uniform float u_Amount;
uniform float u_Scale;

void main()
{
    float noise = (fract(sin(dot(inUV, vec2(u_Amount, u_Amount))) * u_Scale));
    outColor = texture(normals, inUV).rgb - noise * u_Strength;
}

