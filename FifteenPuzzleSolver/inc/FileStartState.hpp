#pragma once
#include "Framework.hpp"

#include "puzzle/BoardHandler.hpp"
#include "includes.h"

#include <fstream>
#include <utility>



// max rozmiar 255x255, chyba starczy
class file_start_state {
    std::vector<uint8_t> table;

public:
    file_start_state(const std::string& file);
    state getState();
};

file_start_state::file_start_state(const std::string& path) : table(16) {
    std::ifstream file;
    file.open(path);
    if (file.is_open()) {
        file >> Board::width >> Board::height;
        Board::width -= '0';
        Board::height -= '0';
        Board::len = Board::width * Board::height;
        uint8_t* cursor = table.data();
        std::string buff;
        for (uint8_t i = 0; i < Board::len; i++, cursor++) {
            file >> buff;
            *cursor = (uint8_t)stoi(buff);
        }
        file.close();
    } else {
        std::cout << "cannot open file\n";
    }
}

state file_start_state::getState() {
    Board::init_same();
    Board b = Board(table);
    // issue: after return is called board destructor, which deallocates table.
    OperationPath p;
    return { b, p };
}