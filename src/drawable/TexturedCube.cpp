#include "TexturedCube.h"

#include "VertexBufferLayout.h"

TexturedCube::TexturedCube()
{
     float vertices[] = {
        
        //pos x 3, normal x 3, tex x 2
        // +Y SIDE
        -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,

        // -Y SIDE
        -0.5f,  -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,

        // +X SIDE
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,

        // -X SIDE
        -0.5f,  0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

        // +Z SIDE
        -0.5f,  0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
         0.5f,  0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

        // -Z SIDE
        -0.5f,  0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        // +X
        8, 9, 10,
        9, 11, 10,

        // -X
        14, 13,	12,
        14, 15, 13,

        // +Y
        1, 2, 0,
        3, 2, 1,

        // -Y
        4, 6, 5,
        5, 6, 7,

        // +Z
        17,	18,	16,
        19,	18,	17,

        // -Z
        20,	22,	21,
        21,	22,	23
    };
    
    mVertexArrayObject = std::make_unique<VertexArrayObject>();
    mIndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]));

    mVertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));

    VertexBufferLayout vertexBufferLayout;
    vertexBufferLayout.push<float>(3);
    vertexBufferLayout.push<float>(3);
    vertexBufferLayout.push<float>(2);
    mVertexArrayObject->addBuffer(*mVertexBuffer, vertexBufferLayout);

    mVertexArrayObject->unbind();
    mIndexBuffer->unbind();
}

TexturedCube::~TexturedCube()
{
    mVertexArrayObject->unbind();
    mIndexBuffer->unbind();
}

void TexturedCube::bind()
{
    mVertexArrayObject->bind();
    mIndexBuffer->bind();
}