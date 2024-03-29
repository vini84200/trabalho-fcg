#version 330 core

const int MAX_LIGHTS = 10;
const uint UseBaseTexture = 0x00000001u;
const uint UseSpecularTexture = 2u;
const uint UseNormalTexture = 4u;
const uint UseShininessTexture = 8u;

struct Light {
    vec3 intensity;
    float ambient;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct Material {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float q;
    uint use_texture;
};

struct PointLight {
    vec3 position;
    Light base;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;
    float radius;
};

layout (location = 0) in vec3 position_modelspace;
layout (location = 2) in vec3 original_normal;
layout (location = 3) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Vetor de luz direcional
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_LIGHTS];
uniform int pointLightsCount;

uniform Material material;

uniform sampler2D baseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularHighlightTexture;

out vec4 cor;

// Calcula a iluminação de um ponto com uma fonte de luz direcional usando o
// modelo de iluminação de Blinn-Phong.
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 Kd, vec3 Ka, vec3 Ks, float q) {
    vec3 lightDir = normalize(-light.direction);
    vec3 halfDir = normalize(lightDir + viewDir);

    // Difusa usando a lei dos cossenos de Lambert
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Especular usando o modelo de iluminação de Blinn-Phong
    float specular = pow(max(dot(normal, halfDir), 0.0), q);

    vec3 diffuseColor = Kd * light.base.intensity * diffuse;
    vec3 specularColor = Ks * light.base.intensity * specular;


    vec3 ambientColor = Ka * light.base.intensity * light.base.ambient;

    return diffuseColor + specularColor + ambientColor;
}

// Calcula a iluminação de um ponto com uma fonte de luz pontual usando o
// modelo de iluminação de Blinn-Phong.
// FONTE: https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/6.multiple_lights.fs
vec3 calcPointLight(vec4 position_world, PointLight light, vec3 normal, vec3 viewDir, vec3 Kd, vec3 Ka, vec3 Ks, float q) {
    vec3 lightDir = normalize(light.position - position_world.xyz);
    vec3 halfDir = normalize(lightDir + viewDir);

    // Difusa usando a lei dos cossenos de Lambert
    float diffuse = max(dot(normal, lightDir), 0.0);

    // Especular usando o modelo de iluminação de Blinn-Phong
    vec3 reflectDir = reflect(-lightDir, normal);
    float specular = pow(max(dot(normal, reflectDir), 0.0), q);

    // Calcula a atenuação da luz
    float distance = length(light.position - position_world.xyz);
    float attenuation = 1.0 / (light.constant_attenuation + light.linear_attenuation * distance + light.quadratic_attenuation * distance * distance);

    vec3 diffuseColor = Kd * light.base.intensity * diffuse * attenuation;
    vec3 specularColor = Ks * light.base.intensity * specular * attenuation;

    vec3 ambientColor = Ka * light.base.intensity * light.base.ambient * attenuation;

    return diffuseColor + specularColor + ambientColor;
}

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
    vec4 v = normalize(camera_position - p);

    cor.a = 1.0f;

    vec3 baseColor = 0u != (material.use_texture & UseBaseTexture)
    ? pow(texture(baseTexture, texcoord_).rgb, vec3(2.2))
    : material.Kd;
    vec3 ambientColor = bool(material.use_texture & UseBaseTexture)
    ? texture(baseTexture, texcoord_).rgb
    : material.Ka;
    float q = bool(material.use_texture & UseShininessTexture)
    ? pow(2, texture(specularHighlightTexture, texcoord_).r * 10.0)
    : material.q;
    vec3 specularColor = bool(material.use_texture & UseSpecularTexture)
    ? texture(specularTexture, texcoord_).rgb
    : material.Ks;

    //    vec3 normalMap = bool(material.use_texture & UseNormalTexture)
    //        ? texture(normalTexture, texcoord_).rgb * 2.0 - 1.0
    //        : vec3(0.0, 0.0, 1.0); // TODO: Fazer isso algum dia quando tiver tempo pra implementar tangentes e bitangentes


    cor.rgb = calcDirectionalLight(directionalLight, n.xyz, v.xyz, baseColor, ambientColor, specularColor, q);
    for (int i = 0; i < pointLightsCount; i++) {
        cor.rgb += calcPointLight(position_world, pointLights[i], n.xyz, v.xyz, baseColor, ambientColor, specularColor, q);
    }
}
