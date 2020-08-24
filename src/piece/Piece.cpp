#include "Piece.h"
#include <iostream>

Piece::Piece(std::vector<std::vector<int>> data, Cell start, std::vector<Cell> wallkick):
    mCurrentPieceData(data),
    cellX(start.x),
    cellY(start.y),
    mWallkick(wallkick)
{
  
    std::cout << "Starting cell: " << cellX << " " << cellY << "\n";

    //initial array defination is out by one rotation due to how the arrays are set up
    //basically [][] != [x][y]
    rotateClockwise(); //hack to get the rows / columns correct
    mRotateState = 0;

    for (int x = 0; x < mCurrentPieceData.size(); ++x)
    {
        mPositions.push_back(std::vector<GameObject>());

        for (int y = 0; y < mCurrentPieceData.at(0).size(); ++y)
        {
            mPositions[x].push_back(GameObject(glm::vec3{ (x * Config::tileWidth) + (cellX * Config::tileWidth), -((y * Config::tileHeight) + (cellY * Config::tileWidth)), 0.0f}));
            mPositions[x].back().setScale(Config::tileWidth, Config::tileHeight, Config::tileHeight);
        }
    }   
}

void Piece::incrementRotationState()
{
    ++mRotateState;
    if(mRotateState > 3)
    {
        mRotateState = 0;
    }
    std::cout << "New rotateState: " << mRotateState << "\n";
}

void Piece::decrementRotationState()
{
    --mRotateState;
    if(mRotateState < 0)
    {
        mRotateState = 3;
    }
    std::cout << "New rotateState: " << mRotateState << "\n";
}

void Piece::drawText()
{
    for (int y = 0; y < mCurrentPieceData.at(0).size(); ++y)
    {
        for (int x = 0; x < mCurrentPieceData.size(); ++x)
        {
            std::cout << mCurrentPieceData[x][y] << " ";
        }

        std::cout << "\n";
    }
    
    std::cout << "\n";
}

void Piece::transpose()
{
    for (int i = 0; i < mCurrentPieceData.size(); ++i)
    {
        for (int j = i; j < mCurrentPieceData.at(i).size(); ++j)
        {
            std::swap(mCurrentPieceData[i][j], mCurrentPieceData[j][i]);
        }
    }
}

void Piece::rotateColumns()
{
    for (int i = 0; i < mCurrentPieceData.size(); i++)
    {
        for (int j = 0, k = mCurrentPieceData.at(i).size() - 1; j < k; j++, k--)
        {
            std::swap(mCurrentPieceData[j][i], mCurrentPieceData[k][i]);
        }
    }
}

void Piece::rotateCounterClockwise()
{
    decrementRotationState();

    //reverse columns
    rotateColumns();

    //transpose
    transpose();
}

void Piece::rotateCounterClockwise(Board& board)
{
    std::cout << "/////////////////////\n";
    std::cout << "Current rotation state: " << mRotateState << "\n";

    //rest is repeated code apart from the rotation and X value / onversion
    rotateCounterClockwise();    

    int tempCellX = cellX;
    int tempCellY = cellY;
    bool rotationFound = false;

    for (int i = 0; i < 5; ++i)
    {
        cellX = tempCellX + -(mWallkick[(mRotateState * 5) + i].x);
        cellY = tempCellY + mWallkick[(mRotateState * 5) + i].y;

        std::cout << "Checking wallkick " << (mRotateState * 5) + i << "\nx: " << -(mWallkick[(mRotateState * 5) + i].x) 
        << "\ny: " << mWallkick[(mRotateState * 5) + i].y << "\n\n";
        if(!isCollision(board))
        {
            rotationFound = true;
            std::cout << "Rotation found in wall kick: " << i << "\n";
            calculateDrawPosition();
            break;
        }
    }

    if(!rotationFound)
    {       
        cellX = tempCellX;
        cellY = tempCellY;
        rotateClockwise();
    }   
}

void Piece::rotateClockwise()
{
    incrementRotationState();

    //transpose
    transpose();

    //reverse columns
    rotateColumns();
}

void Piece::rotateClockwise(Board& board)
{
    std::cout << "/////////////////////\n";
    std::cout << "Current rotation state: " << mRotateState << "\n";

    rotateClockwise();

    int tempCellX = cellX;
    int tempCellY = cellY;
    bool rotationFound = false;

    for (int i = 0; i < 5; ++i)
    {
        cellX = tempCellX + mWallkick[(mRotateState * 5) + i].x;
        cellY = tempCellY + mWallkick[(mRotateState * 5) + i].y;

        std::cout << "Checking wallkick " << (mRotateState * 5) + i << "\nx: " << mWallkick[(mRotateState * 5) + i].x
        << "\ny: " << mWallkick[(mRotateState * 5) + i].y << "\n\n";
        if(!isCollision(board))
        {
            rotationFound = true;
            std::cout << "Rotation found in wall kick: " << i << "\n";
            calculateDrawPosition();
            break;
        }
    }
    if(!rotationFound)
    {       
        cellX = tempCellX;
        cellY = tempCellY;
        rotateCounterClockwise();
    } 
}

bool Piece::moveUp(Board& board)
{
    --cellY;

    //get the right index    
    int indexY = 0;

    for (int x = 0; x < mCurrentPieceData.size(); ++x)
    {
        for (int y = 0; y < mCurrentPieceData.at(0).size(); ++y)
        {
            if(mCurrentPieceData[x][y] != 0)
            {
                indexY = y;

                //std::cout << "IndexY : " << indexY << "\n";

                //bail out
                x = mCurrentPieceData.size();
                y = mCurrentPieceData.at(0).size();
            }
        }
    }  

    if ((cellY + indexY) < 0)
    {
        //std::cout << "Out of bounds\n";
        ++cellY;
        return false;
    }

    //collisions
    if (isCollision(board))
    {
        ++cellY;
        return false;
    }; 

    calculateDrawPosition();

    return true;
}

bool Piece::moveDown(Board& board)
{
    ++cellY;

    //get the right index
    int indexY = 0;

    for (int x = 0; x < mCurrentPieceData.size(); ++x)
    {
        for (int y = mCurrentPieceData.at(0).size() - 1; y >= 0 ; --y)
        {
            if(mCurrentPieceData[x][y] != 0)
            {                
                indexY = y;
             
                //std::cout << "IndexY : " << indexY << "\n";

                //bail out
                x = mCurrentPieceData.size();
                y = 0;
            }
        }
    }  

    if ((cellY + indexY) >= board.getHeight())
    {
        //std::cout << "Out of bounds\n";
        --cellY;
        return false;
    }

    //collisions
    if (isCollision(board))
    {
        --cellY;
        return false;
    }; 

    calculateDrawPosition();

    return true;
}

bool Piece::moveLeft(Board& board)
{
    --cellX;

    //get the right index
    int indexX = 0;    

    for (int x = 0; x < mCurrentPieceData.size(); ++x)
    {
        for (int y = 0; y < mCurrentPieceData.at(0).size(); ++y)
        {
            if(mCurrentPieceData[x][y] != 0)
            {
                indexX = x;

                //std::cout << "IndexX: " << indexX << "\n";

                //bail out
                x = mCurrentPieceData.size();
                y = mCurrentPieceData.at(0).size();                
            }
        }
    }  

    //bounds
    if ((cellX + indexX) < 0)
    {
        //std::cout << "Out of bounds\n";
        ++cellX;
        return false;
    }

    //collisions
    if (isCollision(board))
    {
        ++cellX;
        return false;
    }; 

    calculateDrawPosition();

    return true;
}

bool Piece::moveRight(Board& board)
{
    ++cellX;

    //get the right index
    int indexX = 0;
    int indexY = 0;

    for (int x = mCurrentPieceData.size() - 1; x >= 0; --x)
    {
        for (int y = 0; y < mCurrentPieceData.at(0).size(); ++y)
        {
            if(mCurrentPieceData[x][y] != 0)
            {
                indexX = x;
                indexY = y;

                //std::cout << "IndexX: " << indexX << "\n";
                //std::cout << "IndexY : " << indexY << "\n";

                //bail out
                x = 0;
                y = mCurrentPieceData.at(0).size();
            }
        }
    }  

    if ((cellX + indexX) >= board.getWidth())
    {
        //std::cout << "Out of bounds\n";
        --cellX;
        return false;
    }

    //collisions
    if (isCollision(board))
    {
        --cellX;
        return false;
    }; 

    calculateDrawPosition();

    return true;
}

bool Piece::isCollision(Board& board)
{
    //std::cout << "CellX: " << cellX << "\tCellY: " << cellY << "\n";

    //check for other tiles
    for (int x = 0; x < mCurrentPieceData.size(); ++x)
    {
        for (int y = 0; y < mCurrentPieceData.at(0).size(); ++y)
        {
            if(mCurrentPieceData[x][y] != 0)
            {
                //santity check bounds
                if((cellX + x) < 0)
                {
                    return true;
                }

                if((cellX + x) >= board.getWidth())
                {
                    return true;
                }

                if((cellY + y) < 0)
                {
                    return true;
                }

                if((cellY + y) >= board.getHeight())
                {
                    return true;
                }

                //actual collision
                if(board.getTileType(cellX + x, cellY + y) != 0)
                {
                    //std::cout << "Hit something\n";
 
                    return true;
                }
            }
        }
    }

    return false;
}

void Piece::calculateDrawPosition()
{
    for (int x = 0; x < mCurrentPieceData.size(); ++x)
    {
        for (int y = 0; y < mCurrentPieceData.at(0).size(); ++y)
        {
            mPositions[x][y].setPosition((x * Config::tileWidth) + (cellX * Config::tileWidth), -((y * Config::tileHeight) + (cellY * Config::tileWidth)));            
        }
    }   
}

void Piece::transferPieceToBoard(Board& board)
{
    auto& boardCells = board.getBoard();

    std::cout << "Transferring piece to board\n";

    for (int x = 0; x < mCurrentPieceData.size(); ++x)
    {
        for (int y = 0; y < mCurrentPieceData.at(0).size(); ++y)
        {
            if(mCurrentPieceData[x][y] != 0)
            {
                boardCells[cellX + x][cellY + y].type = mCurrentPieceData[x][y];
            }            
        }
    }   
    board.drawTextBoard();
}