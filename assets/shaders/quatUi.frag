#version 330 core

const uint useTexture = 1u;
const uint useHoverTexture = 2u;

in vec2 TexCoord;
out vec4 outColor;


uniform vec4 color;
uniform uint flags;
uniform bool isHovering;
uniform vec2 textureResize;
uniform vec2 textureOffset;

uniform sampler2D textureBase;
uniform sampler2D textureHover;

void main()
{
    vec2 texCoord = TexCoord * textureResize + textureOffset;

    vec4 texColor = texture(textureBase, texCoord.st);
    vec4 texColorHover = texture(textureHover, texCoord.st);
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
