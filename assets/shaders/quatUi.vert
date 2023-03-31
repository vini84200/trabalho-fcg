#version 330 core
layout (location = 0) in vec3 position_modelspace;

out vec2 TexCoord;

uniform vec2 screen_size;
uniform vec2 mouse_pos;
uniform float time;
uniform vec2 size;
uniform vec2 position;
uniform float z_index;

// Especifico do quat
//uniform vec4 color;

void main()
{

    // Transforma a posição position_modelspace para a posição da tela
    vec2 position_screen = position_modelspace.xy * size + position;

    TexCoord = (position_modelspace.xy + 1)/2;

    gl_Position = vec4(position_screen, z_index, 1.0);
}
