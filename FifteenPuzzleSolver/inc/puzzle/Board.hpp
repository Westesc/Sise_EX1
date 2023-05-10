#pragma once
#include "../Framework.hpp"

#include "IsSame.hpp"

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <utility>

// key of hashmap
struct Board {

    static uint8 length, width, height;

    std::vector<uint8> table;
    uint8 zeroIndex;

    Board(std::vector<uint8> newTable); // NEW
    Board(const Board& other);          // COPY

    bool operator==(const Board& other) const;
    Board& operator=(const Board& other); // trivial

    static void InitializeSame();
    std::string ToString();
};

uint8 Board::length;
uint8 Board::width;
uint8 Board::height;

using State = std::pair<Board, OperationPath>;

Board::Board(
    std::vector<uint8>  table
) : table(std::move(table)) {

    auto it = std::find(this->table.begin(), this->table.end(), 0);
    zeroIndex = it - this->table.begin();                                   // If no zero was found zeroIndex = table.length() !
}

Board::Board(const Board& o) : zeroIndex(o.zeroIndex), table(o.table) {}


void Board::InitializeSame() {

    if (Board::length % 8 == 0) {
        same = &IsSameMod8;
    } else if (Board::length % 4 == 0) {
        same = &IsSameMod4;
    } else {
        same = &IsSameAny;
    }

}


bool Board::operator==(const Board& other) const {
    if (same == &IsSameMod8 || same == &IsSameMod4 || same == &IsSameAny) {
        return same(this->table.data(), other.table.data(), Board::length);
    }
    return false;
}

Board& Board::operator=(const Board& other) {
    zeroIndex = other.zeroIndex;
    table = other.table;
    return *this;
}

std::string Board::ToString() {
    std::string output;

    for (int i = 0; i < Board::length; i++) {
        output += std::to_string(table[i]);
        output += " ";
        if (i % Board::width == Board::width - 1)
            output += '\n';
    }

    return output;
}