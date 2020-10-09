//-----------------------------------------------------------------------------
// basic.frag by Steve Jones 
// Copyright (c) 2015-2016 Game Institute. All Rights Reserved.
//
// Fragment shader 区别就是没字符""了
// frag_color = vec4(0.35f, 0.96f, 0.3f, 1.0f);
//-----------------------------------------------------------------------------
#version 330 core

in vec2 TexCoord;
//uniform vec4 vertColor;
out vec4 frag_color;

uniform sampler2D myTexture;

void main()
{
    // frag_color = vertColor;
	frag_color = texture(myTexture, TexCoord);	// TexCoord 采样基于myTexture
}
