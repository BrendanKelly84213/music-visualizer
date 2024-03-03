#version 330 core
out vec4 FragColor;

in vec4 ourColor;
in vec3 ourPosition;
uniform float uTime;
uniform vec2 uResolution;
uniform float uAmplitude;

void main()
{
    FragColor =ourColor;
}