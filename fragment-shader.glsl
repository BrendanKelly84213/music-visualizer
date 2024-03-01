#version 330 core
out vec4 FragColor;

in vec3 ourColor; // we set this variable in the OpenGL code.
in vec3 ourPosition;

void main()
{
    FragColor = vec4(0.2, 0.1, 0.3, 1.0);
}