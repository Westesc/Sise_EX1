#pragma once
#include "../Framework.hpp"

#include "OperationPath.hpp"
#include "Astar/State.hpp"
#include "BoardHash.hpp"

#include <iostream>
#include <cstdint>
#include <utility>

class BoardHandler {
    BoardHandler() = default; // static class
public:
    static State* NewMoved(const State& old_state, Varieties::Operators op);
    static uint8* NewSolvedTable();
    static void DisplayBoard(const Board& b);
};

uint8* BoardHandler::NewSolvedTable() {
    auto solvedTable = new uint8[Board::length];
    auto solved_cursor = solvedTable;

    for (uint8 i = 1; i < Board::length; i++, solved_cursor++) {
        *solved_cursor = i;
    }

    *solved_cursor = 0; // last element is 0
    return solvedTable;
}

State* BoardHandler::NewMoved(
    const State& oldState, 
    Varieties::Operators Operators
) {
    uint8 movedZeroIndex = oldState.first.zeroIndex;
    uint8 operatorsInt = Operators;

    if (oldState.second.path.empty())
        operatorsInt += 0b100;

    switch (operatorsInt) {

        case Varieties::L: {
            if (oldState.first.zeroIndex % Board::width == 0)
                return nullptr;
            if (*(oldState.second.path.end() - 1) == Varieties::R)
                return nullptr;
            movedZeroIndex--;
        } break;

        case Varieties::R: {
            if (oldState.first.zeroIndex % Board::width == Board::width - 1)
                return nullptr;
            if (*(oldState.second.path.end() - 1) == Varieties::L)
                return nullptr;
            movedZeroIndex++;
        } break;

        case Varieties::U: {
            if (oldState.first.zeroIndex < Board::width)
                return nullptr;
            if (*(oldState.second.path.end() - 1) == Varieties::D)
                return nullptr;
            movedZeroIndex -= Board::width;
        } break;

        case Varieties::D: {
            if (oldState.first.zeroIndex >= (Board::length - Board::width))
                return nullptr;
            if (*(oldState.second.path.end() - 1) == Varieties::U)
                return nullptr;
            movedZeroIndex += Board::width;
        } break;

        case (Varieties::L + 4): {
            if (oldState.first.zeroIndex % Board::width == 0)
                return nullptr;
            movedZeroIndex--;
        } break;

        case (Varieties::R + 4): {
            if (oldState.first.zeroIndex % Board::width == Board::width - 1)
                return nullptr;
            movedZeroIndex++;
        } break;

        case (Varieties::U + 4): {
            if (oldState.first.zeroIndex < Board::width)
                return nullptr;
            movedZeroIndex -= Board::width;
        } break;

        case (Varieties::D + 4): {
            if (oldState.first.zeroIndex >= (Board::length - Board::width))
                return nullptr;
            movedZeroIndex += Board::width;
            break;
        }

        default:
            break;
    }

    Board boardMoved(oldState.first); // throws
    OperationPath pathMoved(oldState.second, Operators);

    uint8* oldZeroPtr = boardMoved.table.data(),
        * newZeroPtr = boardMoved.table.data();

    oldZeroPtr += oldState.first.zeroIndex;
    newZeroPtr += movedZeroIndex;

    *oldZeroPtr = *newZeroPtr;
    *newZeroPtr = 0;

    boardMoved.zeroIndex = movedZeroIndex;

    return new State(boardMoved, pathMoved);
}


void BoardHandler::DisplayBoard(
    const Board& b
) {
    for (int i = 0; i < Board::length; i++) {
        std::cout << +b.table[i] << " ";
        if (i % 4 == 3)
            std::cout << '\n';
    }
}