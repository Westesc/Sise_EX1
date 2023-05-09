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

    static uint8 len, width, height;

    std::vector<uint8> table;
    uint8 zero_idx;

    Board(std::vector<uint8> newTable); // NEW
    Board(const Board& other);          // COPY

    bool operator==(const Board& other) const;
    Board& operator=(const Board& other); // trivial

    static void init_same();
    std::string toString();
};

uint8 Board::len;
uint8 Board::width;
uint8 Board::height;

using state = std::pair<Board, OperationPath>;



bool same_mod8(const uint8_t* solved, const uint8_t* state) {
    auto solved_ptr = (uint64_t*)solved,
        state_ptr = (uint64_t*)state;
    uint8_t steps = Board::len >> 3;
    bool retVal = true;
    for (uint8_t i = 0; i < steps; i++, solved_ptr++, state_ptr++) {
        if ((*solved_ptr ^ *state_ptr) != 0) { // 0 if same
            retVal = false;
        }
    }
    return retVal;
}

bool same_mod4(const uint8_t* solved, const uint8_t* state) {
    auto solved_ptr = (uint32_t*)solved,
        state_ptr = (uint32_t*)state;
    uint8_t steps = Board::len >> 2;
    bool retVal = true;
    for (uint8_t i = 0; i < steps; i++, solved_ptr++, state_ptr++) {
        if (*solved_ptr ^ *state_ptr) {
            retVal = false;
        }
    }
    return retVal;
}

bool same_any(const uint8_t* solved, const uint8_t* state) {
    auto solved_ptr = solved,
        state_ptr = state;

    bool retVal = true;
    for (uint8_t i = 0; i < Board::len; i++, solved_ptr++, state_ptr++) {
        if (*solved_ptr ^ *state_ptr) {
            retVal = false;
        }
    }
    return retVal;
}

Board::Board(std::vector<uint8_t>  table) : table(std::move(table)) {
    auto it = std::find(this->table.begin(), this->table.end(), 0);
    zero_idx = it - this->table.begin(); // if no zero was found zero_idx = table.length()
}

Board::Board(const Board& o) : zero_idx(o.zero_idx), table(o.table) {}


void Board::init_same() {

    if (Board::len % 8 == 0) {
        same = &same_mod8;
    } else if (Board::len % 4 == 0) {
        same = &same_mod4;
    } else {
        same = &same_any;
    }

}


bool Board::operator==(const Board& other) const {
    if (same == &same_mod8 || same == &same_mod4 || same == &same_any) {
        return same(this->table.data(), other.table.data());
    }
    return false;
}

Board& Board::operator=(const Board& other) {
    zero_idx = other.zero_idx;
    table = other.table;
    return *this;
}

std::string Board::toString() {
    std::string ret;
    for (int i = 0; i < Board::len; i++) {
        ret += std::to_string(table[i]);
        ret += " ";
        if (i % Board::width == Board::width - 1)
            ret += '\n';
    }
    return ret;
}