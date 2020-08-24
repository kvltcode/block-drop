#pragma once
#include <vector>
#include <algorithm>
#include "Tile.h"
#include "Config.h"

#include "Board.h"
#include "Cell.h"

//use the SRS system
class Piece
{
private:	
	std::vector<std::vector<int>> mCurrentPieceData;
	std::vector<std::vector<GameObject>> mPositions;
	std::vector<Cell> mWallkick;

	int cellX;
	int cellY;

	int mRotateState { 0 };
	void incrementRotationState();
	void decrementRotationState();

	void transpose();
	void rotateColumns();

	
	void rotateCounterClockwise();
	void rotateClockwise();

public:
	Piece(){}; //creates an empty array. Piece will need set later
	Piece(std::vector<std::vector<int>> data, Cell start, std::vector<Cell> wallkick);
	
	void drawText();

	void rotateCounterClockwise(Board& board);
	void rotateClockwise(Board& board);

	bool moveUp(Board& board);
	bool moveDown(Board& board);
	bool moveLeft(Board& board);
	bool moveRight(Board& board);

	bool isCollision(Board& board);

	void calculateDrawPosition();

	const std::vector<std::vector<int>>& getCurrentPiece() const { return mCurrentPieceData; };
	const glm::mat4& getModel(int x, int y) const { return mPositions[x][y].getModel(); };

	void transferPieceToBoard(Board& board);
	
	//testing
	const glm::vec3& getPosition(int x, int y) const { return mPositions[x][y].getPosition(); }
	const glm::vec3& getScale(int x, int y) const { return mPositions[x][y].getScale(); }

};

