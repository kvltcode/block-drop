#pragma once
#include <glm.hpp>
#include <unordered_map>

struct PieceColours
{
    const std::unordered_map<int, glm::vec4> colour =
    {
		{3, {0.027450980f, 0.988235294f, 0.988235294f, 1.0f}},
		{4, {0.003921569f, 0.003921569f, 0.984313725f, 1.0f}},
		{5, {0.992156863f, 0.666666667f, 0.011764706f, 1.0f}},
		{6, {0.996078431f, 0.992156863f, 0.031372549f, 1.0f}},
		{7, {0.043137255f, 0.941176471f, 0.047058824f, 1.0f}},
		{8, {0.596078431f, 0.007843137f, 0.988235294f, 1.0f}},
		{9, {0.992156863f, 0.007843137f, 0.007843137f, 1.0f}}
    };
};