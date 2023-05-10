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
    const State& oldState,               // board state
    Varieties::Operators presentOperator // direction
) {
    uint8 movedZeroIndex = oldState.first.zeroIndex;    // simply accessing present position of so called '0'. 
    uint8 operatorInt = presentOperator;

    if (oldState.second.path.empty()) // jeœli œcie¿ka dzia³añ nasze "LRUD..." jest pusta inaczej mówi¹c 1 ruch.
        operatorInt += 0b100;

    switch (operatorInt) {

        case Varieties::L: {
            // Nie wyjœæ poza board.
            if (oldState.first.zeroIndex % Board::width == 0)
                return nullptr;
            // Last move Right ? - we can't go back ! 
            if (*(oldState.second.path.end() - 1) == Varieties::R)
                return nullptr;
            movedZeroIndex--;
        } break;

        case Varieties::R: {
            // Nie wyjœæ poza board.
            if (oldState.first.zeroIndex % Board::width == Board::width - 1)
                return nullptr;
            if (*(oldState.second.path.end() - 1) == Varieties::L)
                return nullptr;
            movedZeroIndex++;
        } break;

        case Varieties::U: {
            // Nie wyjœæ poza board.
            if (oldState.first.zeroIndex < Board::width)
                return nullptr;
            if (*(oldState.second.path.end() - 1) == Varieties::D)
                return nullptr;
            movedZeroIndex -= Board::width;
        } break;

        case Varieties::D: {
            // Nie wyjœæ poza board.
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

    Board boardMoved(oldState.first);   // previous board cpy
    OperationPath pathMoved(oldState.second, presentOperator); // emplace used direction

    uint8* oldZeroPtr = boardMoved.table.data(), // point to first element in board
        * newZeroPtr = boardMoved.table.data();

    oldZeroPtr += oldState.first.zeroIndex; // make it point at old '0' position
    newZeroPtr += movedZeroIndex; // make it point at new '0' position

    *oldZeroPtr = *newZeroPtr; // swap their values. 0 <-> newPos
    *newZeroPtr = 0;

    boardMoved.zeroIndex = movedZeroIndex; // but update '0' position and then retrive state !

    return new State(boardMoved, pathMoved);
}


void BoardHandler::DisplayBoard(
    const Board& board
) {
    for (int i = 0; i < Board::length; i++) {
        std::cout << +board.table[i] << " ";
        if (i % 4 == 3)
            std::cout << '\n';
    }
}