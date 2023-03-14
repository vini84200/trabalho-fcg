#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;
in vec2 texcoord_;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 Ka;
uniform vec3 KdIn;
uniform vec3 Ks;
uniform float q;
uniform int texture_;
uniform sampler2D tex;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

void main()
{
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
        //        Kd = vec3(1.0, 0.0, 0.0);
    }
    //    vec3 Kd;// Refletância difusa
    //    vec3 Ks;// Refletância especular
    //    vec3 Ka;// Refletância ambiente
    //    float q;// Expoente especular para o modelo de iluminação de Phong
    //
    //    Kd = vec3(0.08, 0.4, 0.8);
    //    Ks = vec3(0.8, 0.8, 0.8);
    //    Ka = vec3(0.04, 0.2, 0.4);
    //    q = 32.0;

    // Espectro da fonte de iluminação
    vec3 I = vec3(1.0, 1.0, 1.0);

    // Espectro da luz ambiente
    vec3 Ia = vec3(0.02, 0.02, 0.02);

    // Termo difuso utilizando a lei dos cossenos de Lambert
    vec3 lambert_diffuse_term = Kd * I * max(dot(l, n), 0.0);

    // Termo ambiente
    vec3 ambient_term = Ka * Ia;

    // Termo especular utilizando o modelo de iluminação de Phong
    vec3 phong_specular_term  = Ks * I * pow(max(dot(r, v), 0.0), q);

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1.0f;

    // Cor final do fragmento calculada com uma combinação dos termos difuso,
    // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
    color.rgb = lambert_diffuse_term + ambient_term + phong_specular_term;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    //    color.rgb = n.xyz;
    color.rgb = pow(color.rgb, vec3(1.0, 1.0, 1.0)/2.2);
    //    color.rgb = vec3(texcoord_.xy, 0.4);
}


