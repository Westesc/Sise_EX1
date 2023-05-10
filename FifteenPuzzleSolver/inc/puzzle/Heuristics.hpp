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

    auto iterator = state->first.table.begin();
    const uint8* iteratorSolved = solved;

    for (; iterator < state->first.table.end(); iterator++, iteratorSolved++) {
        if (*iterator == 0) // Ignore 0
            continue;
        result += (*iterator != *iteratorSolved);
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

    auto iterator = state->first.table.begin();
    const uint8* iteratorSolved = solved;

    for (; iterator != state->first.table.end() - 2; iterator++, iteratorSolved++) {

        if (*iterator == 0) // Ignore 0
            continue;

        if (*iterator != *iteratorSolved) { // Calculate distance
            result += abs((*iterator - 1) / Board::height - (*iteratorSolved - 1) / Board::height);
            result += abs((*iterator - 1) % Board::width - (*iteratorSolved - 1) % Board::height);
        }

    }

    if (*++iterator != 0) { // Treat last field in a special way
        result += (Board::length - *iterator) / Board::height;
        result += (Board::length - *iterator) % Board::width;
    }

    return result;
}