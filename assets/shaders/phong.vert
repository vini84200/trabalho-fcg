#version 330 core

layout (location = 0) in vec3 position_modelspace;
layout (location = 2) in vec3 original_normal;

out vec4 position_world;
out vec4 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position_modelspace, 1.0);
    position_world = model * vec4(position_modelspace, 1.0);
    normal = inverse(transpose(model)) * vec4(original_normal, 0.0);
    normal.w = 0.0;
}
