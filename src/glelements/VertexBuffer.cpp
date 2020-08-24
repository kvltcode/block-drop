#include "VertexBuffer.h"
#include <glad/glad.h> 

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    glGenBuffers(1, &mVertexBufferID); //how many buffers and a pointer / id
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID); //bind is just selecting the buffer. we tell it the type then pass in the id. doesn't need data straight away
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &mVertexBufferID);
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
