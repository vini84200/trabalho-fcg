#version 330 core

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
in vec4 cor;
out vec4 color;

void main()
{
   color = cor;
}


