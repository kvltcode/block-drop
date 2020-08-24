#pragma once
#include "Cell.h"
#include <glm.hpp>
#include <vector>

class IPiece
{    
  	//interface for I, O, T, S, Z, J, and L
    public:
        
        virtual Cell startingCell() const = 0;
        virtual std::vector<std::vector<int>> data() const = 0;
        virtual std::vector<Cell> wallkick() const = 0;
};