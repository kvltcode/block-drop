#pragma once
#include "IPiece.h"
#include <vector>

class PieceZ: public IPiece
{
	private:
		std::vector<std::vector<int>> mData = 
		{{
			{9, 9, 0},
			{0, 9, 9},
			{0, 0, 0},	
		}};
		
		const Cell mStartingPosition {3,0};
	
		const std::vector<Cell> mWallkick =
		{
			{ 0,  0}, {-1,  0}, {-1,  1}, { 0, -2}, {-1, -2},
			{ 0,  0}, { 1,  0}, { 1, -1}, { 0,  2}, { 1,  2},
			{ 0,  0}, { 1,  0}, { 1,  1}, { 0, -2}, { 1, -2},
			{ 0,  0}, {-1,  0}, {-1, -1}, { 0,  2}, {-1,  2}
		};
	
	public:

		Cell startingCell() const override { return mStartingPosition; };
        std::vector<std::vector<int>> data() const override { return mData; };
		std::vector<Cell> wallkick() const override { return mWallkick; };
};