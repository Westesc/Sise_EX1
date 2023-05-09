#pragma once
#include "../Framework.hpp"

#include "Board.hpp"

struct BoardHash {
    // in my case sizeof(std::size_t) == 8
    std::size_t operator()(const Board& b) const {
        auto ptr = b.table.data();
        std::size_t ret = 0;
        for(uint8_t i = 0; i < Board::len; i++, ptr++) {
            ret ^= ( (std::size_t)(*ptr) << (8 - (i % 8)) * 8 );
        }
        return ret;
    }
};
