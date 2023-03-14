#version 330 core

layout (location = 0) in vec3 position_modelspace;
layout (location = 2) in vec3 original_normal;
layout (location = 3) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 Ka;
uniform vec3 KdIn;
uniform vec3 Ks;
uniform float q;
uniform int texture_;
uniform sampler2D tex;

out vec4 cor;

void main()
{
    gl_Position = projection * view * model * vec4(position_modelspace, 1.0);
    vec4 position_world = model * vec4(position_modelspace, 1.0);
    vec4 normal = inverse(transpose(model)) * vec4(original_normal, 0.0);
    normal.w = 0.0;
    vec2 texcoord_ = texcoord;

    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    vec4 p = position_world;

    vec4 n = normalize(normal);

    vec4 l = normalize(vec4(1.0, 1.0, 0.5, 0.0));

    vec4 v = normalize(camera_position - p);

    vec4 r = 2 * n * dot(n, l) - l;

    vec3 Kd;
    if (texture_ == 1) {
        Kd = texture(tex, texcoord_).rgb;
    } else {
        Kd = KdIn;
    }


    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0, 1.0, 1.0);

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.02, 0.02, 0.02);

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd * I * max(dot(l, n), 0.0);

    // Termo ambiente
    vec3 ambient_term = Ka * Ia;

    // Termo especular utilizando o modelo de iluminação de Phong
    vec3 phong_specular_term = Ks * I * pow(max(dot(r, v), 0.0), q);

    // Alpha default = 1 = 100% opaco = 0% transparente
    cor.a = 1.0f;


    cor.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;


    cor.rgb = pow(cor.rgb, vec3(1.0, 1.0, 1.0) / 2.2);

}
