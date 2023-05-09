#pragma once
#include "../Framework.hpp"


#include "OperationPath.hpp"
#include "Board.hpp"
//#include "../includes.h"

#include <cstdint>

struct Heuristics {
	static uint16_t hamming(state* st, const uint8_t* solved);
	static uint16_t manhattan(state* st, const uint8_t* solved);
};

// Are numbers in place
uint16_t Heuristics::hamming(state* st, const uint8_t* solved) {
    uint16_t ret = 0;
    auto sti = st->first.table.begin();
    const uint8_t* soli = solved;
    for (; sti < st->first.table.end(); sti++, soli++) {
        if (*sti == 0) // Ignore 0
            continue;
        ret += (*sti != *soli);
    }
    return ret;
}

/*
 * idx 0 1 2 3 4 5 .. 14 15
 * val 1 2 3 4 5 6 .. 15  0
 */
// Sum distances from number to expected place
uint16_t Heuristics::manhattan(state* st, const uint8_t* solved) {
    uint16_t ret = 0;
    auto sti = st->first.table.begin();
    const uint8_t* soli = solved;
    for (; sti != st->first.table.end() - 2; sti++, soli++) {
        if (*sti == 0) // Ignore 0
            continue;
        if (*sti != *soli) { // Calculate distance
            ret += abs((*sti - 1) / Board::height - (*soli - 1) / Board::height);
            ret += abs((*sti - 1) % Board::width - (*soli - 1) % Board::height);
        }
    }
    if (*++sti != 0) { // Treat last field in a special way
        ret += (Board::len - *sti) / Board::height;
        ret += (Board::len - *sti) % Board::width;
    }
    return ret;
}