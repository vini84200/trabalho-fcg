#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform bool gammaCorrect;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if (hdr)
    {
        // reinhard
        //         vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        //        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // aproximate filmic curve
        vec3 result = max(vec3(0.0), hdrColor - vec3(0.004));
        result = (result * (6.2 * result + 0.5)) / (result * (6.2 * result + 1.7) + 0.06);
        // also gamma correct while we're at it
        if (gammaCorrect) {
            result = pow(result, vec3(1.0 / gamma));
        }
        FragColor = vec4(result, 1.0);
    }
    else
    {
        vec3 result = pow(hdrColor, vec3(1.0 / gamma));
        FragColor = vec4(result, 1.0);
    }
}