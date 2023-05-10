#pragma once
#include "../Framework.hpp"

#include "OperationPath.hpp"
#include "Board.hpp"

#include <cstdint>

struct Heuristics {
	static uint16 Hamming(State* state, const uint8* solved);
	static uint16 Manhattan(State* state, const uint8* solved);
};

// Are numbers in place
uint16 Heuristics::Hamming(
    State* state, 
    const uint8* solved
) {
    uint16 result = 0;

    auto sti = state->first.table.begin();
    const uint8* soli = solved;

    for (; sti < state->first.table.end(); sti++, soli++) {
        if (*sti == 0) // Ignore 0
            continue;
        result += (*sti != *soli);
    }

    return result;
}

/*
 * idx 0 1 2 3 4 5 .. 14 15
 * val 1 2 3 4 5 6 .. 15  0
 */
// Sums distances from number to expected place
uint16 Heuristics::Manhattan(
    State* state,
    const uint8* solved
) {
    uint16 result = 0;

    auto sti = state->first.table.begin();
    const uint8* soli = solved;

    for (; sti != state->first.table.end() - 2; sti++, soli++) {

        if (*sti == 0) // Ignore 0
            continue;

        if (*sti != *soli) { // Calculate distance
            result += abs((*sti - 1) / Board::height - (*soli - 1) / Board::height);
            result += abs((*sti - 1) % Board::width - (*soli - 1) % Board::height);
        }

    }

    if (*++sti != 0) { // Treat last field in a special way
        result += (Board::length - *sti) / Board::height;
        result += (Board::length - *sti) % Board::width;
    }

    return result;
}