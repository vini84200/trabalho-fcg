#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    // Undo the gamma correction
    FragColor = pow(texture(skybox, TexCoords), vec4(2.2));
    // Do the inverse of a hdr tone mapping
}

// SDR - Gamma Corrected: 0 - 1 float (mentira) - 8 bits -  0 - 255
// SDR - Linear :
// HDR - Linear: 0 - MAX_FLOAT - Meio float - 16 bits -

