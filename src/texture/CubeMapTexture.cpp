#include "CubeMapTexture.h"
#include <glad/glad.h> 
#include "stb_image.h"
#include <iostream>

CubeMapTexture::CubeMapTexture(const std::vector<std::string>& paths)
	: mLocalTextureBuffer(nullptr), mWidth(0), mHeight(0), mBytesPerPixel(0)
{

	stbi_set_flip_vertically_on_load(0); //don't flip

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);

    for (unsigned int i = 0; i < paths.size(); i++)
	{
		mLocalTextureBuffer = stbi_load(paths[i].c_str(), &mWidth, &mHeight, &mBytesPerPixel, 4);

		if (mLocalTextureBuffer)
		{			
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mLocalTextureBuffer);
			std::cout << paths[i] << " loaded\n";
		}
		else
		{
			std::cout << paths[i] << " not loaded\n";
		}
		stbi_image_free(mLocalTextureBuffer);
	}


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeMapTexture::~CubeMapTexture()
{
	glDeleteTextures(1, &mTextureID);
}

void CubeMapTexture::bind() const
{
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
}

void CubeMapTexture::unbind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}