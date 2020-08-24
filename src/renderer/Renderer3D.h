#pragma once
#include <glad/glad.h> 
#include <glm.hpp>


#include <array>

#include "Shader.h"
#include "Camera.h"

#include <ft2build.h>
#include FT_FREETYPE_H 
#include <glad/glad.h> 

#include <map>
#include <unordered_map>
#include <string>

#include "ModelVertex.h"

// This renderer is used for testing
class Renderer3D
{
    private:

		//render stuff
    	static const int cMaxNumberOfCubes;
		static const int cMaxNumberOfVertices;
		static const int cMaxNumberOfIndices;
		static const int cMaxTextureSlots;

		//methods
		static void bufferReset();
		static void flush();

    public:        		
		static void init();
		static void shutdown();
		
		static void begin();		
		static void drawCube(const glm::vec3& position, const glm::vec3& size, int textureID);			
		static void end();		
};