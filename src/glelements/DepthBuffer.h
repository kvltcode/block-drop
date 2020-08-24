#pragma once

class DepthBuffer
{
private:
	const unsigned int mWidth = 2048;
	const unsigned  mHeight = 2048;

    unsigned int mDepthMapFBO;
    unsigned int mDepthMapTexture; //actual texture

public:
	DepthBuffer();

	void setUp() const;

	const unsigned int getDepthMapTexture() const { return mDepthMapTexture; }
};