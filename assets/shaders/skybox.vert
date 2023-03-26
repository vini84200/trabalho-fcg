#version 330 core
layout (location = 0) in vec3 position_modelspace;
layout (location = 2) in vec3 original_normal;
layout (location = 3) in vec2 texcoord;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = position_modelspace;
    gl_Position = projection * view * vec4(position_modelspace, 1.0);
}