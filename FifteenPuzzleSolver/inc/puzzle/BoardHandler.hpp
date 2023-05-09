#pragma once
#include "../Framework.hpp"

#include "OperationPath.hpp"
#include "AstarState.hpp"
#include "BoardHash.hpp"

#include <iostream>
#include <cstdint>
#include <utility>

class BoardHandler {
    BoardHandler() = default; // static class
public:
    static State* NewMoved(const State& old_state, ops::operators op);
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

State* BoardHandler::NewMoved(const State& old_state, ops::operators op) {
    uint8 moved_zero_idx = old_state.first.zeroIndex;
    uint8 op_int = op;

    if (old_state.second.path.empty())
        op_int += 0b100;

    switch (op_int) {
        case ops::L:
            if (old_state.first.zeroIndex % Board::width == 0)
                return nullptr;
            if (*(old_state.second.path.end() - 1) == ops::R)
                return nullptr;
            moved_zero_idx--;
            break;
        case ops::R:
            if (old_state.first.zeroIndex % Board::width == Board::width - 1)
                return nullptr;
            if (*(old_state.second.path.end() - 1) == ops::L)
                return nullptr;
            moved_zero_idx++;
            break;
        case ops::U:
            if (old_state.first.zeroIndex < Board::width)
                return nullptr;
            if (*(old_state.second.path.end() - 1) == ops::D)
                return nullptr;
            moved_zero_idx -= Board::width;
            break;
        case ops::D:
            if (old_state.first.zeroIndex >= (Board::length - Board::width))
                return nullptr;
            if (*(old_state.second.path.end() - 1) == ops::U)
                return nullptr;
            moved_zero_idx += Board::width;
            break;
        case (ops::L + 4):
            if (old_state.first.zeroIndex % Board::width == 0)
                return nullptr;
            moved_zero_idx--;
            break;
        case (ops::R + 4):
            if (old_state.first.zeroIndex % Board::width == Board::width - 1)
                return nullptr;
            moved_zero_idx++;
            break;
        case (ops::U + 4):
            if (old_state.first.zeroIndex < Board::width)
                return nullptr;
            moved_zero_idx -= Board::width;
            break;
        case (ops::D + 4):
            if (old_state.first.zeroIndex >= (Board::length - Board::width))
                return nullptr;
            moved_zero_idx += Board::width;
            break;
        default:
            break;
    }

    Board moved_board(old_state.first); // throws
    OperationPath moved_path(old_state.second, op);

    uint8* ptr_oz = moved_board.table.data(),
        * ptr_nz = moved_board.table.data();
    ptr_oz += old_state.first.zeroIndex;
    ptr_nz += moved_zero_idx;
    *ptr_oz = *ptr_nz;
    *ptr_nz = 0;
    moved_board.zeroIndex = moved_zero_idx;

    return new State(moved_board, moved_path);
}


void BoardHandler::DisplayBoard(const Board& b) {
    for (int i = 0; i < Board::length; i++) {
        std::cout << +b.table[i] << " ";
        if (i % 4 == 3)
            std::cout << '\n';
    }
}