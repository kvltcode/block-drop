#pragma once

#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"

#include <memory>

class Cube
{
private:
	std::unique_ptr<VertexArrayObject> mVertexArrayObject;
	std::unique_ptr<IndexBuffer> mIndexBuffer;
	std::unique_ptr<VertexBuffer> mVertexBuffer;

public:
	Cube();
	~Cube();

	Cube(const Cube&) 					= delete;
	Cube& operator=(const Cube& other)  = delete;	    
    Cube(Cube&&)               			= delete;
    Cube& operator=(Cube&&)    			= delete;

	const unsigned int getIndexBufferCount() const
	{
		return mIndexBuffer->getCount();
	}

	void bind();

};