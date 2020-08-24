#include "VertexArrayObject.h"
#include "VertexBufferLayout.h"

#include <glad/glad.h> 



VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &mVertexArrayObjectID);
	//glBindVertexArray(mVertexArrayObjectID);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &mVertexArrayObjectID);
}

void VertexArrayObject::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();

	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		//can do this anytime after the buffer is bound
		glEnableVertexAttribArray(i); //tell it to actually enable to attribute array
		glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.getStride(), (const void*)offset); //this code links the buffer to the VAO
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}


	/*
		glCall(glEnableVertexAttribArray(0)); //tell it to actually enable to attribute array

		glCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));

		glCall(glEnableVertexAttribArray(1)); //tell it to actually enable to attribute array

		glCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2* sizeof(float))));
	*/
}

void VertexArrayObject::bind() const
{
	glBindVertexArray(mVertexArrayObjectID);
}
void VertexArrayObject::unbind() const
{
	glBindVertexArray(0);
}