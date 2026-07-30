#version 460 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    FragColor = texture(screenTexture, TexCoords);
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}