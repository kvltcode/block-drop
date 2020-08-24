#include "IndexBuffer.h"
#include <glad/glad.h> 


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :mCount(count)
{
    glGenBuffers(1, &mIndexBufferID); //how many buffers and a pointer / id
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID); //bind is just selecting the buffer. we tell it the type then pass in the id. doesn't need data straight away
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &mIndexBufferID);
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferID);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}