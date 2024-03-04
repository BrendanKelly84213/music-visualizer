#version 330 core
out vec4 FragColor;

in vec3 ourPosition;
uniform float uTime;
uniform vec2 uResolution;
uniform float uAmplitude;

void main()
{
    vec2 uv = gl_FragCoord.xy / uResolution;
    FragColor = vec4(uv, 0, 1);
}