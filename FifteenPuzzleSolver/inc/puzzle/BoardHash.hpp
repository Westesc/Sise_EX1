#pragma once
#include "../Framework.hpp"

#include "Board.hpp"

struct BoardHash {
    size operator()(const Board& board) const {
        auto ptr = board.table.data();
        size output = 0;

        for(uint8 i = 0; i < Board::length; i++, ptr++)
            output ^= ( (size)(*ptr) << (8 - (i % 8)) * 8 );

        return output;
    }
};
