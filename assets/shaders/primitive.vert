#version 330 core

layout (location = 0) in vec3 position_modelspace;
layout (location = 1) in vec4 color;

out vec4 cor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position_modelspace, 1.0);
    cor = color;
}
