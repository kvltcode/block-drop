#pragma once
#include <glm.hpp>
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
