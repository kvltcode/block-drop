#include "Texture.h"
#include <glad/glad.h> 
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path)
	: mLocalTextureBuffer(nullptr), mWidth(0), mHeight(0), mBytesPerPixel(0)
{

	stbi_set_flip_vertically_on_load(1); //makes it upside down because openGL thinks stuff starts at the bottom left (at least in png)

	mLocalTextureBuffer = stbi_load(path.c_str(), &mWidth, &mHeight, &mBytesPerPixel, 4);
	
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);	
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalTextureBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (mLocalTextureBuffer)
	{
		stbi_image_free(mLocalTextureBuffer);
		std::cout << path << " loaded\n";
	}
	else
	{
		std::cout << path << " not loaded\n";
	}
	
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}