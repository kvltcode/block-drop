#pragma once
#include <glm.hpp>

struct QuadVertex
{
	glm::vec3 position;
	glm::vec4 colour;
	glm::vec2 texCoord;
	float textureIndex;
};