#pragma once

#include "Config.h"

#include "Tile.h"
#include <vector>
#include <array>

/*
Key

Empty
I
O
T
S
Z
J
L
ERASE
*/
class Board
{
private:	
	int mBoardWidth;
	int mBoardHeight;

	std::vector<std::vector<Tile>> mBoard;
	std::array<GameObject, 3> mBorder =
	{
		GameObject(glm::vec3{ 0.0f, 0.0f, 0.0f}),
		GameObject(glm::vec3{ 0.0f, 0.0f, 0.0f}),
		GameObject(glm::vec3{ 0.0f, 0.0f, 0.0f})
	};

	std::array<glm::vec4, 12> mColours =
	{
		glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f},
		glm::vec4{ 0.0f, 0.0f, 0.5f, 1.0f},
		glm::vec4{ 1.0f, 0.0f, 1.0f, 1.0f},
		glm::vec4{ 0.5f, 0.0f, 1.0f, 1.0f},
		glm::vec4{ 0.0f, 0.0f, 1.0f, 1.0f},
		glm::vec4{ 0.0f, 0.5f, 1.0f, 1.0f},
		glm::vec4{ 0.0f, 1.0f, 1.0f, 1.0f},
		glm::vec4{ 0.0f, 1.0f, 0.5f, 1.0f},
		glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f},
		glm::vec4{ 0.5f, 1.0f, 0.0f, 1.0f},
		glm::vec4{ 1.0f, 1.0f, 0.0f, 1.0f},
		glm::vec4{ 1.0f, 0.5f, 0.0f, 1.0f}
	};
	
public:
	Board(int width, int height);

	std::vector<std::vector<Tile>>& getBoard() { return mBoard; };
	std::array<GameObject, 3>& getBorder() { return mBorder; }

	void drawTextBoard();

	const int getTileType(int x, int y) const { return mBoard[x][y].type;};
	const int getWidth() const { return mBoardWidth; };
	const int getHeight() const { return mBoardHeight; };

	const int checkForCompletedRows();

	const glm::vec4& getBoardColour(int currentLevel) const;

	void reset();

};

