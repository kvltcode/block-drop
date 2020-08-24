#pragma once
#include <glad/glad.h> 
#include <glm.hpp>


#include <array>

#include "Shader.h"
#include "Camera.h"
#include "QuadVertex.h"

#include <ft2build.h>
#include FT_FREETYPE_H 
#include <glad/glad.h> 

#include <map>
#include <unordered_map>
#include <string>

struct Character
{
	glm::vec2 pixelSize;
	glm::vec2 baseLineOffsetPixels;
	float pixelXOffset;
	
	float normalisedTexCoordX;
	float normalisedTexCoordY;
	float normalisedTexCoordWidth;
	float normalisedTexCoordHeight;
};

struct FontSet
{
	unsigned int textureID;		
	float textureWidth;
	float textureHeight;
    std::map<GLchar, Character> characters;    
};

class TextRenderer
{
    private:

		//render stuff
    	static const int cMaxNumberOfQuads;
		static const int cMaxNumberOfVertices;
		static const int cMaxNumberOfIndices;
		static const int cMaxTextureSlots;

		//font stuff	
		static std::unordered_map<std::string, FontSet> sFontSet;

		//font config
    	static glm::vec4 sFontColour;
		static std::string sCurrentFont;

		//methods
		static void bufferReset();
		static void flush();
		//anchor point is bottom left
		static void drawTextQuad(const glm::vec2& position, const glm::vec2& size, const float texX, const float texY, const float texWidth, const float texHeight, const int textureID = 0);

    public:        		
		static void init();
		static void shutdown();
		static bool loadFontFace(std::string name, const char* path);
		
		static void begin(std::unique_ptr<Shader>& shader, const glm::mat4& projectionView);
		static void renderText(const std::string& text, glm::vec2& position, const float scale);
		static void end();		
				
		static void setFont(const std::string& font);
		static void setFontColour(const glm::vec4& colour);
		static const float getStringWidth(const std::string& text, const float scale);
		
		//used for testing to make sure the full font texture exists
		static void renderFullFontTexture(const glm::vec2& position);
};