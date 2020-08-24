#include "Cube.h"

#include "VertexBufferLayout.h"


Cube::Cube()
{
    //positions + colours
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,     0.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f,
    };

    unsigned int indices[] = { //6*6
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };

    mVertexArrayObject = std::make_unique<VertexArrayObject>();
    mIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]));

    mVertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

    VertexBufferLayout vertexBufferLayout;
    vertexBufferLayout.push<float>(3);
    vertexBufferLayout.push<float>(3);
    mVertexArrayObject->addBuffer(*mVertexBuffer, vertexBufferLayout);

    mVertexArrayObject->unbind();
    mIndexBuffer->unbind();
}

Cube::~Cube()
{
    mVertexArrayObject->unbind();
    mIndexBuffer->unbind();
}

void Cube::bind()
{
    mVertexArrayObject->bind();
    mIndexBuffer->bind();
}