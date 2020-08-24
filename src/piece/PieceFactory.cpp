#include "PieceFactory.h"
#include "Random.h"
#include <iostream>

Piece PieceFactory::getNewPiece()
{   
    int selection =  Random::GetInstance().drawNumber(1,7);
    return getNewPiece(selection);
}

Piece PieceFactory::getNewPiece(int selection)
{
    std::unique_ptr<IPiece> selectedPiece;

   	//I, O, T, S, Z, J, and L. 
    switch(selection)   
    {
        case 1: // I
            selectedPiece = std::make_unique<PieceI>();
            std::cout << "Generated: I\n";
        break;
        case 2: // O
            selectedPiece = std::make_unique<PieceO>();
            std::cout << "Generated: O\n";
        break;
        case 3: // T
            selectedPiece = std::make_unique<PieceT>();
            std::cout << "Generated: T\n";
        break;
        case 4: // S
            selectedPiece = std::make_unique<PieceS>();
            std::cout << "Generated: S\n";
        break;
        case 5: // Z
            selectedPiece = std::make_unique<PieceZ>();
            std::cout << "Generated: Z\n";
        break;
        case 6: // J
            selectedPiece = std::make_unique<PieceJ>();
            std::cout << "Generated: J\n";
        break;
        case 7: // L
            selectedPiece = std::make_unique<PieceL>();
            std::cout << "Generated: L\n";
        break;
    }

    Piece returnPiece(
        selectedPiece->data(), 
        selectedPiece->startingCell(), 
        selectedPiece->wallkick()   
    );

    return returnPiece;
}