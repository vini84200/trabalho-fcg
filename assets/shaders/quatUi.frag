#version 330 core
in vec4 FragColor;
out vec4 outColor;

uniform vec4 color;

void main()
{
    outColor = color;
}
