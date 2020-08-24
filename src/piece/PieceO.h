#pragma once
#include "IPiece.h"
#include <vector>

class PieceO: public IPiece
{
	private:
		std::vector<std::vector<int>> mData = 
		{{
			{6, 6},
			{6, 6},	
		}};
		
		const Cell mStartingPosition {4,0};
	
		const std::vector<Cell> mWallkick =
		{
			{ 0,  0}, { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0},
			{ 0,  0}, { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0},
			{ 0,  0}, { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0},
			{ 0,  0}, { 0,  0}, { 0,  0}, { 0,  0}, { 0,  0},
		};
	
	public:

		Cell startingCell() const override { return mStartingPosition; };
        std::vector<std::vector<int>> data() const override { return mData; };
		std::vector<Cell> wallkick() const override { return mWallkick; };
};