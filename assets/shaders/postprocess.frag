#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

const int NO_HDR = 0;
const int REINHARD = 1;
const int EXTENDED_REINHARD = 2;
const int EXPOSURE = 3;
const int FILMIC = 4;
const int UNCHARTED2 = 5;
const int ACES = 6;
const int ACES2 = 7;
const int REINHARD_EXTENDED_LUMINANCE = 8;

uniform sampler2D hdrBuffer;
uniform int hdr;
uniform bool gammaCorrect;
uniform float exposure;


float luminance(vec3 color) {
    return dot(color, vec3(0.2126, 0.7152, 0.0722));
}

vec3 changeLuminance(vec3 color, float newLuminance) {
    float oldLuminance = luminance(color);
    if (oldLuminance == 0.0) {
        return vec3(0.0);
    }
    return color * (newLuminance / oldLuminance);
}

vec3 uncharted2Tonemap(vec3 x) {
    float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

const mat3 aces_input = mat3(
    0.59719f, 0.35458f, 0.04823f,
    0.07600f, 0.90834f, 0.01566f,
    0.02840f, 0.13383f, 0.83777f
);
const mat3 aces_output = mat3(
     1.60475f, -0.53108f, -0.07367f,
    -0.10208f,  1.10813f, -0.00605f,
    -0.00327f, -0.07276f,  1.07602f
);

vec3 mul(const mat3 m, vec3 v)
{
    float x = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
    float y = m[1][0] * v[0] + m[1][1] * v[1] + m[1][2] * v[2];
    float z = m[2][0] * v[0] + m[2][1] * v[1] + m[2][2] * v[2];
    return vec3(x, y, z);
}

vec3 rtt_and_odt_fit(vec3 v) {
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}


void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 result = hdrColor;

    // HDR tone mapping
    if (hdr == REINHARD) {
        result = hdrColor / (hdrColor + vec3(1.0));
    }
    if (hdr == EXTENDED_REINHARD) {
        float maxWhite = exposure;
        vec3 numerator = hdrColor * (vec3(1.0) + (hdrColor / vec3(maxWhite * maxWhite)));
        result = numerator / (vec3(1.0) + hdrColor);
    }
    if (hdr == REINHARD_EXTENDED_LUMINANCE) {
        float l_old = luminance(hdrColor);
        float numerator = l_old * (1.0 + l_old / (exposure * exposure));
        float l_new = numerator / (1.0 + l_old);
        result = changeLuminance(hdrColor, l_new);
    }
    if (hdr == EXPOSURE) {
        result = vec3(1.0) - exp(-hdrColor * exposure);
    }
    if (hdr == FILMIC) {
        // aproximate filmic curve
        result = max(vec3(0.0), hdrColor - vec3(0.004));
        result = (result * (6.2 * result + 0.5)) / (result * (6.2 * result + 1.7) + 0.06);
    }
    if (hdr == UNCHARTED2) {
        float exposureBias = 2.0 * exposure;
        vec3 curr = uncharted2Tonemap(hdrColor * exposureBias);

        vec3 W = vec3(11.2, 11.2, 11.2);
        vec3 whiteScale = vec3(1.0) / uncharted2Tonemap(W);
        result = curr * whiteScale;
    }
    if (hdr == ACES) {
        // ACES Filmic Tone Mapping (Narkowicz)
        // https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
        float a = 2.51;
        float b = 0.03;
        float c = 2.43;
        float d = 0.59;
        float e = 0.14;
        result = clamp((hdrColor * (a * hdrColor + b)) / (hdrColor * (c * hdrColor + d) + e), 0.0, 1.0);
    }
    if (hdr == ACES2) {
        // ACES Filmic Tone Mapping (Hill)
        hdrColor = mul(aces_input, hdrColor);
        hdrColor = rtt_and_odt_fit(hdrColor);
        result = mul(aces_output, hdrColor);
    }

    result = clamp(result, 0.0, 1.0);

    // GAMMA CORRECTION
    if (gammaCorrect) {
        result = pow(result, vec3(1.0 / gamma));
    }

    FragColor = vec4(result, 1.0);
}