#pragma once
#include "../Framework.hpp"

#include "Board.hpp"

struct BoardHash {
    size operator()(
        const Board& board
    ) const {
        auto tablePtr = board.table.data();
        size output = 0;

        for(uint8 i = 0; i < Board::length; i++, tablePtr++)
            output ^= ( (size)(*tablePtr) << (8 - (i % 8)) * 8 );

        return output;
    }
};
