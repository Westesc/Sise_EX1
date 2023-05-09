#pragma once
#include "Framework.hpp"

#include "puzzle/BoardHandler.hpp"

#include <fstream>
#include <utility>

// max rozmiar 255x255, chyba starczy
class FileStartState {
    std::vector<uint8> table;

public:
    FileStartState(const std::string& file);
    State GetState();
};

FileStartState::FileStartState(
    const std::string& path
) : table(16) {

    std::ifstream file;
    file.open(path);

    if (file.is_open()) {

        file >> Board::width >> Board::height;
        Board::width -= '0';
        Board::height -= '0';
        Board::length = Board::width * Board::height;
        uint8* cursor = table.data();
        std::string buff;
        for (uint8 i = 0; i < Board::length; i++, cursor++) {
            file >> buff;
            *cursor = (uint8)stoi(buff);
        }
        file.close();

    } else {
        std::cout << "FileStartState: Unable to open file!\n";
    }

}

State FileStartState::GetState() {
    Board::InitializeSame();
    Board board = Board(table);
    OperationPath operationPath {};

    return { board, operationPath };
}