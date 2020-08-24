#pragma once

#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"

#include <memory>

class Quad
{
private:
	std::unique_ptr<VertexArrayObject> mVertexArrayObject;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
	std::unique_ptr<VertexBuffer> mVertexBuffer;

public:
	Quad();
	~Quad();

	Quad(const Quad&) 					= delete;
	Quad& operator=(const Quad& other)  = delete;	    
    Quad(Quad&&)               			= delete;
    Quad& operator=(Quad&&)    			= delete;

	const unsigned int getIndexBufferCount() const
	{
		return mIndexBuffer->getCount();
	}

	void bind();
};

