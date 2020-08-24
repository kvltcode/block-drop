#pragma once
#include <glm.hpp>

struct ModelVertex
{
	glm::vec3 position;
	glm::vec4 colour;
    glm::vec3 normal;
	glm::vec2 texCoord;
	float textureIndex;
    //pos x 3, normal x 3, tex x 2
};