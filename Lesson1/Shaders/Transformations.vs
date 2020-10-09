#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;
  
uniform mat4 transform;

void main()
{
	// UV坐标0 ~ 1 映射到 OpenGL中的坐标-0.5 ~ 0.5（范围是-1 ~ 1） 坐标变换的是OpenGL坐标，平移0.5，屏幕中移动了一半
    gl_Position = transform * vec4(position, 1.0f);
    ourColor = color;
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
} 