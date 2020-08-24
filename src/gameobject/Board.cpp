#include "Board.h"
#include <iostream>



Board::Board(int width, int height):
mBoardWidth(width),
mBoardHeight(height)
{
    //init the board with tiles
    for (int x = 0; x < mBoardWidth; ++x)
    {
        mBoard.push_back(std::vector<Tile>());

        for (int y = 0; y < mBoardHeight; ++y)
        {
            int type = 0; //default to zero for empty

            bool drawBounds = false;
            if(drawBounds) //testing to see boundary collisions
            {            
                if (x == 1 || x == 8)
                {
                    type = 2;
                }

                if (y == 1 || y == 18)
                {
                    type = 2;
                }                
            }
      
            mBoard[x].push_back(Tile{type, GameObject(glm::vec3{ (x * Config::tileWidth) , -(y * Config::tileHeight), 0.0f})});
            mBoard[x].back().object.setScale(Config::tileWidth, Config::tileHeight, Config::tileHeight);
        }
    }

    //set up the border frame objects (set up as three cuboids)
    mBorder[0].setPositionX((-Config::tileWidth) + (Config::tileWidth / 4.0f));
    mBorder[0].setPositionY(-((Config::tileHeight * (mBoardHeight-1)) /2.0f));
    mBorder[0].setScale(Config::tileWidth / 2.0f, Config::tileHeight * mBoardHeight, Config::tileHeight);
    
    mBorder[1].setPositionX((Config::tileWidth * (mBoardWidth-1)) / 2.0f);
    mBorder[1].setPositionY(-(Config::tileHeight * mBoardHeight) + (Config::tileHeight / 4.0f));
    mBorder[1].setScale(Config::tileWidth * (mBoardWidth + 1), Config::tileHeight / 2.0f, Config::tileHeight * 5);

    mBorder[2].setPositionX((Config::tileWidth * mBoardWidth) - (Config::tileWidth / 4.0f));
    mBorder[2].setPositionY(-((Config::tileHeight * (mBoardHeight-1)) /2.0f));
    mBorder[2].setScale((Config::tileWidth + 2) / 2.0f, Config::tileHeight * mBoardHeight, Config::tileHeight);    
}

//text output for debugging
void Board::drawTextBoard()
{
    for (int y = 0; y < mBoardHeight; ++y)
    {
        for (int x = 0; x < mBoardWidth; ++x)
        {
            std::cout << mBoard[x][y].type << " ";
        }

        std::cout << "\n";
    }
    std::cout << "\n";
}

//going to want to split this function up to just check and do nothing else if I add in AI
const int Board::checkForCompletedRows()
{
    int yStart = mBoardHeight - 1;
    int numberOfCompletedRows = 0;

    //check if any rows are full and if they are, make them empty
    for (int y = yStart; y > 0; --y)
    {
        bool completedRow = true;

        for (int x = 0; x < mBoardWidth; ++x)
        {
            if(mBoard[x][y].type == 0)
            {
                completedRow = false;
                break;
            }           
        }    

        if (completedRow)
        {
            for (int x = 0; x < mBoardWidth; ++x)
            {
                //probably want to handle animation here first
                mBoard[x][y].type = 0;                               
            }
            
            ++numberOfCompletedRows; //could just use a vector so we know what rows are actually complete
            std::cout <<"Row completed\n";
            //drawTextBoard();
        }
    }

    //move rows down all empty rows
    int count = 0;
    while (count < mBoardHeight - 1) //stupid algorithm but works
    {
        for (int y = mBoardHeight - 1; y > 1; --y)
        {
            bool emptyRow = true;

            for (int x = 0; x < mBoardWidth; ++x)
            {
                if(mBoard[x][y].type != 0)
                {
                    emptyRow = false;
                    break;
                }           
            }    

            if (emptyRow)
            {
                for (int x = 0; x < mBoardWidth; ++x)
                {
                    //probably want to handle animation here first
                    mBoard[x][y].type = mBoard[x][y-1].type;         
                    mBoard[x][y-1].type = 0;                      
                }          
            }
        }
        ++count;
    }

    //drawTextBoard();   

    return numberOfCompletedRows;
}

//colour chosen by level
//loop the colours by the number of available colours
const glm::vec4& Board::getBoardColour(int currentLevel) const
{
    int cap = mColours.size();
    while(currentLevel >= cap)
    {
       currentLevel -= cap; 
    }

    return mColours[currentLevel];
}

//clear the board to empty
void Board::reset()
{
    for (int x = 0; x < mBoardWidth; ++x)
    {
        for (int y = 0; y < mBoardHeight; ++y)
        {
            mBoard[x][y].type = 0;
        }
    }
}