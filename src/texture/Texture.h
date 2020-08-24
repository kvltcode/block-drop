#pragma once
#include <string>
#include <vector>

class Texture
{
private:
	unsigned char* mLocalTextureBuffer;
	int mWidth, mHeight, mBytesPerPixel;

	unsigned int mTextureID;

public:
	Texture(const std::string& path);
	//Texture(const std::vector<std::string>& paths);
	~Texture();

	void bind() const;
	void unbind();

	const unsigned int getID() const { return mTextureID; };

	int getWidth() { return mWidth; }
	int getHeight() { return mHeight; }
};

