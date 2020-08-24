#pragma once

#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"

#include <memory>

class TexturedQuad
{
private:
	std::unique_ptr<VertexArrayObject> mVertexArrayObject;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
	std::unique_ptr<VertexBuffer> mVertexBuffer;

public:
	TexturedQuad();
	~TexturedQuad();

	TexturedQuad(const TexturedQuad&) 					= delete;
	TexturedQuad& operator=(const TexturedQuad& other)  = delete;	    
    TexturedQuad(TexturedQuad&&)               			= delete;
    TexturedQuad& operator=(TexturedQuad&&)    			= delete;

	const unsigned int getIndexBufferCount() const
	{
		return mIndexBuffer->getCount();
	}

	void bind();
};

