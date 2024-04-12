// Author @patriciogv - 2015
// http://patriciogonzalezvivo.com

#version 330 core

#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;
// Properties
uniform float u_octaves;
uniform float u_lacunarity;
uniform float u_gain;
//
// Initial values
uniform float u_initialAmplitude;
uniform float u_initialFrequency;

uniform float u_speed;
uniform float u_something;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
                 43758.5453123);
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

float fbm (in vec2 st) {
    float value = 0.0;
    float amplitude = u_initialAmplitude;
    float frequency = u_initialFrequency;

    for (int i = 0; i < u_octaves; i++) {
        value += amplitude * noise(frequency*st);
        frequency *= u_lacunarity;
        amplitude *= u_gain;
    }
    return value;
}

void main() {
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    vec2 q = vec2(0.);
    q.x = fbm( st + u_speed*u_time * u_something);
    q.y = fbm( st + vec2(1.0));

    vec2 r = vec2(0.);
    r.x = fbm( st + 1.0*q + vec2(1.7,9.2)+ u_something*u_time );
    r.y = fbm( st + 1.0*q + vec2(8.3,2.8));

    vec3 color = vec3(0.0);
    color += fbm(st + fbm(st + u_time * u_something) + u_time * u_speed);
    float f = fbm(st + r);
    color += vec3(0.137,0.283,0.495);
    color = mix(vec3(0.137,0.283,0.495), vec3(0.667, 0.667, 0.667), clamp(f*f * 4.0, 0.0, 1.0));
    color += f;

    gl_FragColor = vec4(color,1.0);
}
