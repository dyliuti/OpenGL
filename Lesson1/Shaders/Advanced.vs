#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    color = texture(screenTexture, TexCoords);
    float average = (color.r + color.g + color.b) / 3.0;
    color = vec4(average, average, average, 1.0);
}