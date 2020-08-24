#pragma once

#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"

#include <memory>

class TexturedCube
{
private:
	std::unique_ptr<VertexArrayObject> mVertexArrayObject;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
	std::unique_ptr<VertexBuffer> mVertexBuffer;
	
public:
	TexturedCube();
	~TexturedCube();

	TexturedCube(const TexturedCube&) 					= delete;
	TexturedCube& operator=(const TexturedCube& other)  = delete;	    
    TexturedCube(TexturedCube&&)               			= delete;
    TexturedCube& operator=(TexturedCube&&)    			= delete;

	const unsigned int getIndexBufferCount() const
	{
		return mIndexBuffer->getCount();
	}

	void bind();
};

