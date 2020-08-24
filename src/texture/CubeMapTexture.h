#pragma once
#include <string>
#include <vector>

class CubeMapTexture
{
private:
	unsigned char* mLocalTextureBuffer;
	int mWidth, mHeight, mBytesPerPixel;

	unsigned int mTextureID;

public:
	//CubeMapTexture(const std::string& path);
	CubeMapTexture(const std::vector<std::string>& paths);
	~CubeMapTexture();

	void bind() const;
	void unbind();

	const unsigned int getID() const { return mTextureID; };

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }
};

