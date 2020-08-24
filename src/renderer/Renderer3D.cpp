#include "Renderer3D.h"
#include "RenderStats.h"
#include <iostream>

const int Renderer3D::cMaxNumberOfCubes = 1000;
const int Renderer3D::cMaxNumberOfVertices = cMaxNumberOfCubes * 24; //because it's cube. each vertice has 13 elements
const int Renderer3D::cMaxNumberOfIndices = cMaxNumberOfCubes * 36;
const int Renderer3D::cMaxTextureSlots = 32; 

//share this stuff with other renderers
struct RendererDataModel // this is instead of member variables
{
    GLuint vertexArray = 0;
    GLuint vertexBuffer = 0;
    GLuint indexBuffer = 0;

    int indexCount = 0;

    ModelVertex* cubeBuffer = nullptr; //cpu buffer
    ModelVertex* cubeBufferPosition = nullptr;    

    std::array<int, 32> textureSlots; //sort this
};

//storage for the data for renderer
static RendererDataModel sRendererDataModel;


void Renderer3D::init()
{
    sRendererDataModel.cubeBuffer = new ModelVertex[cMaxNumberOfVertices];

  	glGenVertexArrays(1, &sRendererDataModel.vertexArray);
    glBindVertexArray(sRendererDataModel.vertexArray);

    glGenBuffers(1, &sRendererDataModel.vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sRendererDataModel.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ModelVertex) * cMaxNumberOfVertices , nullptr, GL_DYNAMIC_DRAW); //nullptr means it isn't getting set just now

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (const void*)offsetof(ModelVertex, position)); //don't techically need, because it'll be zero

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (const void*)offsetof(ModelVertex, colour));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (const void*)offsetof(ModelVertex, normal));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (const void*)offsetof(ModelVertex, texCoord));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (const void*)offsetof(ModelVertex, textureIndex));

    int indices[cMaxNumberOfIndices];
    int offset = 0;
    for (int i = 0; i < cMaxNumberOfIndices; i += 36)
    {
        // +X
        //8, 9, 10,
        //9, 11, 10,
        indices[i + 0] = 8 + offset;
        indices[i + 1] = 9 + offset;
        indices[i + 2] = 10 + offset;

        indices[i + 3] = 9 + offset;
        indices[i + 4] = 11 + offset;
        indices[i + 5] = 10 + offset;       

        // -X
        //14, 13,	12,
        //14, 15, 13,
        indices[i + 6] = 14 + offset;
        indices[i + 7] = 13 + offset;
        indices[i + 8] = 12 + offset;

        indices[i + 9] = 14 + offset;
        indices[i + 10] = 15 + offset;
        indices[i + 11] = 13 + offset;

        // +Y
        //1, 2, 0,
        //3, 2, 1,
        indices[i + 12] = 1 + offset;
        indices[i + 13] = 2 + offset;
        indices[i + 14] = 0 + offset;

        indices[i + 15] = 3 + offset;
        indices[i + 16] = 2 + offset;
        indices[i + 17] = 1 + offset;

        // -Y
        //4, 6, 5,
        //5, 6, 7,
        indices[i + 18] = 4 + offset;
        indices[i + 19] = 6 + offset;
        indices[i + 20] = 5 + offset;

        indices[i + 21] = 5 + offset;
        indices[i + 22] = 6 + offset;
        indices[i + 23] = 7 + offset;

        // +Z
        //17,	18,	16,
        //19,	18,	17,
        indices[i + 24] = 17 + offset;
        indices[i + 25] = 18 + offset;
        indices[i + 26] = 16 + offset;

        indices[i + 27] = 19 + offset;
        indices[i + 28] = 18 + offset;
        indices[i + 29] = 17 + offset;

        // -Z
        //20,	22,	21,
        //21,	22,	23
        indices[i + 30] = 20 + offset;
        indices[i + 31] = 22 + offset;
        indices[i + 32] = 21 + offset;

        indices[i + 33] = 21 + offset;
        indices[i + 34] = 22 + offset;
        indices[i + 35] = 23 + offset;


        offset += 24; //cos cube
    }
    
    glGenBuffers(1, &sRendererDataModel.indexBuffer); //how many buffers and a pointer / id
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sRendererDataModel.indexBuffer); //bind is just selecting the buffer. we tell it the type then pass in the id. doesn't need data straight away
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //static is fine because it doesn't get changed  

    for (int i = 1; i < cMaxTextureSlots; ++i)
    {
        sRendererDataModel.textureSlots[i] = 0; //all textures set to zero
    }
}

void Renderer3D::shutdown()
{
    glDeleteVertexArrays(1, &sRendererDataModel.vertexArray);
    glDeleteBuffers(1, &sRendererDataModel.vertexBuffer);
    glDeleteBuffers(1, &sRendererDataModel.indexBuffer);

    delete[] sRendererDataModel.cubeBuffer;
}

//entry point
//see if this stuff can be thrown in the init
void Renderer3D::begin()
{

    //shader->setUniformMat4f("u_ProjectionView", projectionView);
    //shader->setUniform1i("u_texture", 0);

    //check
    //bind texture outside of this
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, sFontSet[sCurrentFont].textureID);

    bufferReset();
}

//buffer reset
void Renderer3D::bufferReset()
{
    //set the index position to the start of the cube buffer    
    sRendererDataModel.cubeBufferPosition = sRendererDataModel.cubeBuffer;
}

void Renderer3D::end()
{
    //see if this can be done another way
    //it's basically size of vertex * position in the buffer
    GLsizeiptr bufferSize = (uint8_t*)sRendererDataModel.cubeBufferPosition - (uint8_t*)sRendererDataModel.cubeBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, sRendererDataModel.vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, sRendererDataModel.cubeBuffer);

    flush();
}

//copy buffer onto the GPU
void Renderer3D::flush()
{
   //for (int i = 0; i < sRendererDataModel.textureSlotIndex; ++i)
   //for (int i = 0; i < 2; ++i)
   //{
        //don't need to do this here because we're doing it outside the renderer

   //     glBindTextureUnit(i, sRendererDataModel.textureSlots[i]);
  // }

    glBindVertexArray(sRendererDataModel.vertexArray);
    glDrawElements(GL_TRIANGLES, sRendererDataModel.indexCount, GL_UNSIGNED_INT, nullptr); //number of indices

    sRendererDataModel.indexCount = 0;

    renderstats::drawCount++;

    //glBindTexture(GL_TEXTURE_2D, 0);
}

//now a cube
void Renderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, int textureID)
{
    if (sRendererDataModel.indexCount >= cMaxNumberOfIndices)// || sRendererDataModel.textureSlotIndex > 31)  
    {
        end();
        bufferReset();
    }

    //actually set this below
    constexpr glm::vec4 colour = { 1.0f, 1.0f, 1.0f, 1.0f }; //plain white. could be be used for tint

     // +Y SIDE
    //-0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
    //0.5f,  0.5f, -0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
    //-0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
    //0.5f,  0.5f,  0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y + size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        colour;
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y + size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 0.0f, 1.0f, 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y + size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 1.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y + size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    // -Y SIDE
    //-0.5f,  -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
    //0.5f,  -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
    //-0.5f,  -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
    //0.5f,  -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y - size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        colour;
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, -1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y - size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 0.0f, 1.0f, 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, -1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y - size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 1.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, -1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y - size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, -1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    // +X SIDE
    //0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    //0.5f,  0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    //0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    //0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y + size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        colour;
    sRendererDataModel.cubeBufferPosition->normal =        { 1.0f, 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y + size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 0.0f, 1.0f, 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 1.0f, 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y - size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 1.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 1.0f, 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y - size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 1.0f, 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    // -X SIDE
    //-0.5f,  0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
    //-0.5f,  0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    //-0.5f, -0.5f,  0.5f,    -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
    //-0.5f, -0.5f, -0.5f,    -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y + size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        colour;
    sRendererDataModel.cubeBufferPosition->normal =        { -1.0f, 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y + size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 0.0f, 1.0f, 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { -1.0f, 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y - size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 1.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { -1.0f, 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y - size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { -1.0f, 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    // +Z SIDE
    //-0.5f,  0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
    //0.5f,  0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
    //-0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    //0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y + size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        colour;
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y + size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 0.0f, 1.0f, 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y - size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 1.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y - size.y, position.z + size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    // -Z SIDE
    //-0.5f,  0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
    //0.5f,  0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
    //-0.5f, -0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
    //0.5f, -0.5f, -0.5f,     0.0f, 0.0f, -1.0f,  0.0f, 1.0f
    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y + size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        colour;
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y + size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 0.0f, 1.0f, 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 0.0f, -1.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 0.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x - size.x, position.y - size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 1.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 0.0f, -1.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 1.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;

    sRendererDataModel.cubeBufferPosition->position =      { position.x + size.x, position.y - size.y, position.z - size.z};
    sRendererDataModel.cubeBufferPosition->colour =        { 1.0f, 0.0f, 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->normal =        { 0.0f, 0.0f, -1.0f };
    sRendererDataModel.cubeBufferPosition->texCoord =      { 0.0f, 1.0f };
    sRendererDataModel.cubeBufferPosition->textureIndex =  textureID;
    sRendererDataModel.cubeBufferPosition++;
    sRendererDataModel.indexCount += 36;
    renderstats::cubeCount++;
    renderstats::verticeCount += 24;
}