#include "TexturedQuad.h"

#include "VertexBufferLayout.h"


TexturedQuad::TexturedQuad()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    mVertexArrayObject = std::make_unique<VertexArrayObject>();
    mIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]));

    mVertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

    VertexBufferLayout vertexBufferLayout;
    vertexBufferLayout.push<float>(3);
    vertexBufferLayout.push<float>(2);
    mVertexArrayObject->addBuffer(*mVertexBuffer, vertexBufferLayout);

    mVertexArrayObject->unbind();
    mIndexBuffer->unbind();
}

TexturedQuad::~TexturedQuad()
{
    mVertexArrayObject->unbind();
    mIndexBuffer->unbind();
}

void TexturedQuad::bind()
{
    mVertexArrayObject->bind();
    mIndexBuffer->bind();
}

