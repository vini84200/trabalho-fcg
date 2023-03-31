#version 330 core

const uint useTexture = 1u;
const uint useHoverTexture = 2u;

in vec2 TexCoord;
out vec4 outColor;


uniform vec4 color;
uniform uint flags;
uniform bool isHovering;

uniform sampler2D texture;
uniform sampler2D textureHover;

void main()
{
    vec4 texColor = texture2D(texture, TexCoord.st);
    vec4 texColorHover = texture2D(textureHover, TexCoord.st);
    vec4 finalColor = color;
    if (isHovering && (flags & useHoverTexture) != 0u)
    {
        finalColor = texColorHover;
    }
    else if ((flags & useTexture) != 0u)
    {
        finalColor = texColor;
    }
    outColor = finalColor;
}
