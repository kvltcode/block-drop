#pragma once
#include <vector>
#include <memory>

#include "IPiece.h"
#include "Piece.h"

#include "PieceI.h"
#include "PieceO.h"
#include "PieceT.h"
#include "PieceS.h"
#include "PieceZ.h"
#include "PieceJ.h"
#include "PieceL.h"

class PieceFactory
{
public:
	Piece getNewPiece();
	Piece getNewPiece(int selection);
};

