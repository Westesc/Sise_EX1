#pragma once
#include "../Framework.hpp"

//#include "../includes.h"

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <utility>

using Same = bool(*)(const uint8*, const uint8*);
Same same = nullptr;

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



bool SameMod8(const uint8* solved, const uint8* State) {
    auto solved_ptr = (uint64*)solved,
        state_ptr = (uint64*)State;
    uint8 steps = Board::length >> 3;
    bool retVal = true;
    for (uint8 i = 0; i < steps; i++, solved_ptr++, state_ptr++) {
        if ((*solved_ptr ^ *state_ptr) != 0) { // 0 if same
            retVal = false;
        }
    }
    return retVal;
}

bool SameMod4(const uint8* solved, const uint8* State) {
    auto solved_ptr = (uint32*)solved,
        state_ptr = (uint32*)State;
    uint8_t steps = Board::length >> 2;
    bool retVal = true;
    for (uint8 i = 0; i < steps; i++, solved_ptr++, state_ptr++) {
        if (*solved_ptr ^ *state_ptr) {
            retVal = false;
        }
    }
    return retVal;
}

bool SameAny(const uint8* solved, const uint8* State) {
    auto solved_ptr = solved,
        state_ptr = State;

    bool retVal = true;
    for (uint8 i = 0; i < Board::length; i++, solved_ptr++, state_ptr++) {
        if (*solved_ptr ^ *state_ptr) {
            retVal = false;
        }
    }
    return retVal;
}

Board::Board(std::vector<uint8>  table) : table(std::move(table)) {
    auto it = std::find(this->table.begin(), this->table.end(), 0);
    zeroIndex = it - this->table.begin(); // if no zero was found zero_idx = table.length()
}

Board::Board(const Board& o) : zeroIndex(o.zeroIndex), table(o.table) {}


void Board::InitializeSame() {

    if (Board::length % 8 == 0) {
        same = &SameMod8;
    } else if (Board::length % 4 == 0) {
        same = &SameMod4;
    } else {
        same = &SameAny;
    }

}


bool Board::operator==(const Board& other) const {
    if (same == &SameMod8 || same == &SameMod4 || same == &SameAny) {
        return same(this->table.data(), other.table.data());
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