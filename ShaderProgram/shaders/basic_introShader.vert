//-----------------------------------------------------------------------------
// basic.vert by Steve Jones 
// Copyright (c) 2015-2016 Game Institute. All Rights Reserved.
//
// Vertex shader  区别就是没字符""了  in type(vec3) variable name(pos)
// layout (location = 0) 只在GLSL3.3版本中有 3.2 或更低都没  location = 0 表示vec3 的属性 
//-----------------------------------------------------------------------------
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;

//uniform vec2 posOffset;

out vec2 TexCoord;

void main()
{
	//gl_Position = vec4(pos.x + posOffset.x, pos.y + posOffset.y, pos.z, 1.0);
	gl_Position = vec4(pos.x , pos.y , pos.z, 1.0);
	TexCoord = texCoord;
}