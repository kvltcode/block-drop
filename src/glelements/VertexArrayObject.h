#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout; //forward declration

//ties vertex buffer with vertex layout
class VertexArrayObject
{
private:
	unsigned int mVertexArrayObjectID;
public:
	VertexArrayObject();
	~VertexArrayObject();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void bind() const;
	void unbind() const;
};