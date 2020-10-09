#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform float mixValue;

// Texture samplers
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
	// Linearly interpolate between both textures (second texture is only slightly combined)
	// color = texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0f);
	// (1-alpha) * src + alpha * dst
	color = mix(texture(ourTexture1, TexCoord) * vec4(ourColor, 1.0f), texture(ourTexture2, TexCoord), mixValue);
}