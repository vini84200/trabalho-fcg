#version 330 core

layout (location = 0) in vec4 NDC_coefficients;
layout (location = 1) in vec4 color_coefficients;

out vec4 cor_interpolada_pelo_rasterizador;

void main()
{
    gl_Position = NDC_coefficients;
    cor_interpolada_pelo_rasterizador = color_coefficients;
}
