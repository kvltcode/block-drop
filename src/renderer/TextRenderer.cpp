#include "TextRenderer.h"
#include "RenderStats.h"
#include <iostream>

const int TextRenderer::cMaxNumberOfQuads = 1000;
const int TextRenderer::cMaxNumberOfVertices = cMaxNumberOfQuads * 4; //because it's square. each vertice has 10 elements
const int TextRenderer::cMaxNumberOfIndices = cMaxNumberOfQuads * 6;

std::unordered_map<std::string, FontSet> TextRenderer::sFontSet;

glm::vec4 TextRenderer::sFontColour = { 1.0f, 1.0f, 1.0f, 1.0f };
std::string TextRenderer::sCurrentFont = "Default";


//share this stuff with other renderers
struct RendererData // this is instead of member variables
{
    GLuint vertexArray = 0;
    GLuint vertexBuffer = 0;
    GLuint indexBuffer = 0;

    int indexCount = 0;

    QuadVertex* quadBuffer = nullptr; //cpu buffer
    QuadVertex* quadBufferPosition = nullptr;
};

//storage for the data for renderer
static RendererData sRenderData;


void TextRenderer::init()
{
    sRenderData.quadBuffer = new QuadVertex[cMaxNumberOfVertices];

  	glGenVertexArrays(1, &sRenderData.vertexArray);
    glBindVertexArray(sRenderData.vertexArray);

    glGenBuffers(1, &sRenderData.vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, sRenderData.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * cMaxNumberOfVertices , nullptr, GL_DYNAMIC_DRAW); //nullptr means it isn't getting set just now

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, position)); //don't techically need, because it'll be zero

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, colour));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, texCoord));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), (const void*)offsetof(QuadVertex, textureIndex));

    int indices[cMaxNumberOfIndices];
    int offset = 0;
    for (int i = 0; i < cMaxNumberOfIndices; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;

        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset +=4; //cos square
    }
    
    glGenBuffers(1, &sRenderData.indexBuffer); //how many buffers and a pointer / id
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sRenderData.indexBuffer); //bind is just selecting the buffer. we tell it the type then pass in the id. doesn't need data straight away
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //static is fine because it doesn't get changed  
}

void TextRenderer::shutdown()
{
    glDeleteVertexArrays(1, &sRenderData.vertexArray);
    glDeleteBuffers(1, &sRenderData.vertexBuffer);
    glDeleteBuffers(1, &sRenderData.indexBuffer);

    for (auto& font: sFontSet)
    {
        glDeleteTextures(1, &font.second.textureID);
    }

    delete[] sRenderData.quadBuffer;
}

//entry point
//see if this stuff can be thrown in the init
void TextRenderer::begin(std::unique_ptr<Shader>& shader, const glm::mat4& projectionView)
{
    shader->bind();

    shader->setUniformMat4f("u_ProjectionView", projectionView);
    shader->setUniform1i("u_texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sFontSet[sCurrentFont].textureID);

    bufferReset();
}

//buffer reset
void TextRenderer::bufferReset()
{
    //set the index position to the start of the quad buffer    
    sRenderData.quadBufferPosition = sRenderData.quadBuffer;
}

void TextRenderer::end()
{
    //see if this can be done another way
    //it's basically size of vertex * position in the buffer
    GLsizeiptr bufferSize = (uint8_t*)sRenderData.quadBufferPosition - (uint8_t*)sRenderData.quadBuffer;
    glBindBuffer(GL_ARRAY_BUFFER, sRenderData.vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize, sRenderData.quadBuffer);

    flush();
}

//copy buffer onto the GPU
void TextRenderer::flush()
{
    glBindVertexArray(sRenderData.vertexArray);
    glDrawElements(GL_TRIANGLES, sRenderData.indexCount, GL_UNSIGNED_INT, nullptr); //number of indices

    sRenderData.indexCount = 0;

    renderstats::drawCount++;

    glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::drawTextQuad(const glm::vec2& position, const glm::vec2& size, const float texX, const float texY, const float texWidth, const float texHeight, const int textureID /* =0  */)
{
    if (sRenderData.indexCount >= cMaxNumberOfIndices)
    {
        end();
        bufferReset();
    }

    sRenderData.quadBufferPosition->position =      { position.x, position.y, 0.0f };
    sRenderData.quadBufferPosition->colour =        sFontColour;
    sRenderData.quadBufferPosition->texCoord =      { texX, texY };
    sRenderData.quadBufferPosition->textureIndex =  textureID;
    sRenderData.quadBufferPosition++;

    sRenderData.quadBufferPosition->position =      { position.x + size.x, position.y, 0.0f };
    sRenderData.quadBufferPosition->colour =        sFontColour;
    sRenderData.quadBufferPosition->texCoord =      { texX + texWidth, texY };
    sRenderData.quadBufferPosition->textureIndex =  textureID;
    sRenderData.quadBufferPosition++;

    sRenderData.quadBufferPosition->position =      { position.x + size.x, position.y - size.y, 0.0f };
    sRenderData.quadBufferPosition->colour =        sFontColour;
    sRenderData.quadBufferPosition->texCoord =      { texX + texWidth, texY + texHeight };
    sRenderData.quadBufferPosition->textureIndex =  textureID;
    sRenderData.quadBufferPosition++;

    sRenderData.quadBufferPosition->position =      { position.x, position.y - size.y, 0.0f };
    sRenderData.quadBufferPosition->colour =        sFontColour;
    sRenderData.quadBufferPosition->texCoord =      { texX, texY + texHeight };
    sRenderData.quadBufferPosition->textureIndex =  textureID;
    sRenderData.quadBufferPosition++;

    sRenderData.indexCount += 6;
    renderstats::quadCount++;
    renderstats::verticeCount += 4;
}

void TextRenderer::renderFullFontTexture(const glm::vec2& positon)
{
    drawTextQuad(glm::vec2(positon.x,positon.y), 
        glm::vec2(sFontSet[sCurrentFont].textureWidth, sFontSet[sCurrentFont].textureHeight), 
        0.0f, 0.0f, 1.0f, 1.0f);
}

void TextRenderer::renderText(const std::string& text, glm::vec2& position, float scale)
{
    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = sFontSet[sCurrentFont].characters[*c];
        
        glm::vec2 characterPosition = {
            position.x + (ch.baseLineOffsetPixels.x * scale),
            position.y + (ch.baseLineOffsetPixels.y * scale)
        };

        glm::vec2 characterSize = {
            ch.pixelSize.x * scale,
            ch.pixelSize.y * scale
        };
        
        drawTextQuad(characterPosition, 
            characterSize, 
            ch.normalisedTexCoordX, ch.normalisedTexCoordY, ch.normalisedTexCoordWidth, ch.normalisedTexCoordHeight);

        // advance cursors for next glyph
        position.x += ch.pixelXOffset* scale;
    } 
}

bool TextRenderer::loadFontFace(std::string name, const char* path)
{
    FT_Library freetypeLibrary;

    //freetype
    if (FT_Init_FreeType(&freetypeLibrary))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    //https://community.khronos.org/t/how-to-use-gltexsubimage2d/19273
    //https://docs.gl/gl4/glTexSubImage2D
    FT_Face face;
    if (FT_New_Face(freetypeLibrary, path, 0, &face)) ///todo - add to resource manager and handle errors properly
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_Face(face);
        return false;
    }

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    std::cout << "Number of glypths" << face->num_glyphs << "\n";

    float textureWidth = 0;
	float textureHeight = 0; 
    
    const int firstCharacter = 32;
    const int lastCharacter = 128;

    // work out the max texture sizes by looping through each glyph
    for (GLubyte c = firstCharacter; c < lastCharacter; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        textureWidth += face->glyph->bitmap.width;
        textureHeight = std::max(static_cast<unsigned int>(textureHeight), face->glyph->bitmap.rows);
    }

    //glActiveTexture(GL_TEXTURE0);
    unsigned int fullFontTexture;
    glGenTextures(1, &fullFontTexture);
    glBindTexture(GL_TEXTURE_2D, fullFontTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    // Clamping to edges to prevent artifacts when scaling and linear filtering usually looks best for text
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    

    //map each glyph to struct
    std::map<GLchar, Character> characters;    
    //use the bitmap buffer data of each glyph to create a complete texture
    float xPosition = 0;
    for (GLubyte c = firstCharacter; c < lastCharacter; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        //build up the texture
        glTexSubImage2D(GL_TEXTURE_2D, 0, xPosition, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // store character for later use
        //tex width +1 and -2 values are tweaks because of artifacts showing up on the side by side glyphs
        Character character = {
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), //pixelSize
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), //baseLineOffsetPixels
            face->glyph->advance.x >> 6, //pixelXOffset to next char. bitshiffted
            (xPosition + 1) / textureWidth, //normalisedTexCoordX
            0, //normalisedTexCoordY
            static_cast<float>(face->glyph->bitmap.width - 2) / static_cast<float>(textureWidth), //normalisedTexCoordWidth
            static_cast<float>(face->glyph->bitmap.rows / static_cast<float>(textureHeight)) //normalisedTexCoordHeight
        };
        characters.insert(std::pair<GLchar, Character>(c, character));

        xPosition += face->glyph->bitmap.width;
    }
    glBindTexture( GL_TEXTURE_2D, 0);

    //add font
    sFontSet[name] = { 
        fullFontTexture,
        textureWidth,
        textureHeight,
        characters
      };

    FT_Done_Face(face);

    // Destroy FreeType once we're finished    
    FT_Done_FreeType(freetypeLibrary);

    return true;
}

void TextRenderer::setFont(const std::string& font)
{
    if (sFontSet.find(font) != sFontSet.end())
    {
        sCurrentFont = font;
         //bind here because changing font changes the texture
         //if it wasn't done here it'll need to be done every render
        glBindTexture(GL_TEXTURE_2D, sFontSet[sCurrentFont].textureID);
    }
}

void TextRenderer::setFontColour(const glm::vec4& colour)
{
    sFontColour = colour;
}

const float TextRenderer::getStringWidth(const std::string& text, const float scale)
{
    float width = 0.0f;
  
    for (int i = 0; i < text.size(); ++i)    
    {
        Character ch = sFontSet[sCurrentFont].characters[text[i]];

        width += ch.pixelXOffset * scale; 
    }

    return width;
}