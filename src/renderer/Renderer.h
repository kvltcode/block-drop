#pragma once


#include "Quad.h"
#include "TexturedQuad.h"
#include "Cube.h"
#include "TexturedCube.h"
#include "DepthBuffer.h"
#include "Shader.h"

#include <memory>
#include <string>

enum class RenderBuffer
{
	DEFAULT,
	DEPTH
};

class Renderer
{
public:

	static Renderer& GetInstance()
	{
		static Renderer sInstance;
		return sInstance;
	};
	
	static bool initialised;

	void init(unsigned int width, unsigned int height);
	
	void setupBuffer(RenderBuffer renderBuffer = RenderBuffer::DEFAULT);
	const unsigned int getBufferTexture(RenderBuffer renderBuffer = RenderBuffer::DEFAULT) const;

	void drawQuad();
	void drawTexturedQuad();
	void drawCube();
	void drawTexturedCube();

private:
	Renderer() {};

	Renderer(const Renderer&) 					= delete;
	Renderer& operator=(const Renderer& other)  = delete;	    
    Renderer(Renderer&&)               			= delete;
    Renderer& operator=(Renderer&&)    			= delete;

	std::unique_ptr<Quad> mQuad;
	std::unique_ptr<TexturedQuad> mTexturedQuad;
	std::unique_ptr<Cube> mCube;
	std::unique_ptr<TexturedCube> mTexturedCube;
	std::unique_ptr<DepthBuffer> mDepthBuffer;
};
