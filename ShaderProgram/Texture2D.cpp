//-----------------------------------------------------------------------------
// Texture2D.cpp by Steve Jones 
// Copyright (c) 2015-2019 Game Institute. All Rights Reserved.
//
// Simple 2D texture class
//-----------------------------------------------------------------------------
#include "Texture2D.h"
#include <iostream>
#include <cassert>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include <memory>

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------
Texture2D::Texture2D()
	: mTexture(0)
{
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
Texture2D::~Texture2D()
{
	glDeleteTextures(1, &mTexture);
}

void free_test(void* retval_from_stbi_load) {
	stbi_image_free(retval_from_stbi_load);
}

//-----------------------------------------------------------------------------
// Load a texture with a given filename using stb image loader
// http://nothings.org/stb_image.h
// Creates mip maps if generateMipMaps is true.
//-----------------------------------------------------------------------------
bool Texture2D::loadTexture(const string& fileName, bool generateMipMaps)
{
	int width, height, components;
	// 使用stbi image library加载图片  从磁盘读取，然后分配内存并返回指针指向那个内存
	unsigned char* imageData2 = stbi_load(fileName.c_str(), &width, &height, &components, STBI_rgb_alpha);
	std::shared_ptr<unsigned char> imageData = std::shared_ptr<unsigned char>(imageData2, free_test);  // 直接去掉imageData = std::shared_ptr<unsigned char>也行
	if (imageData == NULL)
	{
		std::cerr << "Error loading texture '" << fileName << "'" << std::endl;
		return false;
	}

	// 转换图像
	int widthInBytes = width * 4;
	unsigned char *top = NULL;	// 0-255
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int halfHeight = height / 2;
	for (int row = 0; row < halfHeight; row++)
	{
		top = imageData.get() + row * widthInBytes;
		bottom = imageData.get() + (height - row - 1) * widthInBytes;
		for (int col = 0; col < widthInBytes; col++)
		{ 
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture); // all upcoming GL_TEXTURE_2D operations will affect our texture object (mTexture)

	// Set the texture wrapping/filtering options (on the currently bound texture object)
	// GL_CLAMP_TO_EDGE
	// GL_REPEAT
	// GL_MIRRORED_REPEAT
	// GL_CLAMP_TO_BORDER
	// GL_LINEAR
	// GL_NEAREST
	// GLenum target, GLenum pname, GLint param
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels
	// level 0:获取全分辨率纹理，而不是小的
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.get());

	if (generateMipMaps)
		glGenerateMipmap(GL_TEXTURE_2D);		// 不会改变我们已经加载的图片，但是openGL创建的2D纹理将是mipMpas

	//stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind texture when done so we don't accidentally mess up our mTexture

	return true;
}

//-----------------------------------------------------------------------------
// Bind the texture unit passed in as the active texture in the shader
//-----------------------------------------------------------------------------
void Texture2D::bind(GLuint texUnit)
{
	assert(texUnit >= 0 && texUnit < 32);

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, mTexture);
}