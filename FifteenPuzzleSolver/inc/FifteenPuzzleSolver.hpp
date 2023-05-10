#pragma once
#include "Framework.hpp"

#include "FileStartState.hpp"
#include "InfoBundle.hpp"
#include "strategies/Astar.hpp"
#include "strategies/Bfs.hpp"
#include "strategies/Dfs.hpp"

#include <iomanip>

class FifteenPuzzleSolver {
    InfoBundle info;
    string strategy, params, 
        fileStart, fileResult, fileExtra;

    static Varieties::Operators* GetOrder(string state);
    static Varieties::Heuristics GetHeuristic(string state);

public:
    FifteenPuzzleSolver(string newStrategy, string newParams, string newFileStart, string newFileResult, string newFileExtra);
    FifteenPuzzleSolver(char** arguments);

    void FindSolution();
};

FifteenPuzzleSolver::FifteenPuzzleSolver(char** argv) : info(), strategy(argv[1]), params(argv[2]), fileStart(argv[3]), fileResult(argv[4]), fileExtra(argv[5]) {}

FifteenPuzzleSolver::FifteenPuzzleSolver(string newStrategy, string newParams, string newFileStart, string newFileResult, string newFileExtra) :
    info(), strategy(std::move(newStrategy)), params(std::move(newParams)), fileStart(std::move(newFileStart)),
    fileResult(std::move(newFileResult)), fileExtra(std::move(newFileExtra)) {}

Varieties::Operators* FifteenPuzzleSolver::GetOrder(
    string state
) {
    if (state.size() == 4) {
        auto order = new Varieties::Operators[4];
        for (int i = 0; i < 4; i++) {
            switch (state[i]) {
                case 'L':
                case 'l':
                    order[i] = Varieties::L;
                    break;
                case 'R':
                case 'r':
                    order[i] = Varieties::R;
                    break;
                case 'U':
                case 'u':
                    order[i] = Varieties::U;
                    break;
                case 'D':
                case 'd':
                    order[i] = Varieties::D;
                    break;
                default:
                    throw std::logic_error("illegal operators, must be permutation of L, R, U, D");
            }
        }
        return order;
    }
    throw std::logic_error("incorrect operators count");
}

Varieties::Heuristics FifteenPuzzleSolver::GetHeuristic(
    string state
) {
    std::transform(state.begin(), state.end(), state.begin(), ::tolower);
    if (state == "hamm") {
        return Varieties::hamm;
    }
    if (state == "manh") {
        return Varieties::manh;
    }
    return Varieties::error;
}

void FifteenPuzzleSolver::FindSolution() {

    FileStartState startStateHandler(fileStart);
    const State startState = startStateHandler.GetState();
    OperationPath solution;

    if (strcmp(strategy.c_str(), "bfs") == 0) {
        Varieties::Operators* order = GetOrder(params);

        Board::InitializeSame();
        solvedTable = BoardHandler::NewSolvedTable();

        solution = bfs(startState, order, info);

        delete[] solvedTable;
        delete order;

    } else if (strcmp(strategy.c_str(), "dfs") == 0) {
        Varieties::Operators* order = GetOrder(params);

        Board::InitializeSame();
        solvedTable = BoardHandler::NewSolvedTable();

        solution = dfs(startState, order, info);

        delete[] solvedTable;
        delete order;

    } else if (strcmp(strategy.c_str(), "astr") == 0) {
        Varieties::Heuristics heuristic = GetHeuristic(params);

        Board::InitializeSame();
        solvedTable = BoardHandler::NewSolvedTable();

        solution = astr(startState, heuristic, info);

        delete[] solvedTable;
    }

    const double executionTime = info.GetTime();

    { // Result File
        std::ofstream fileSolution(fileResult);
        fileSolution << solution.GetLength() << '\n'
            << solution.GetString() << '\n';
        fileSolution.close();
    }

    { // Extra info file 
        std::ofstream fileInfoExtra(fileExtra);
        fileInfoExtra << solution.GetLength() << '\n'
            << info.processed << '\n'
            << info.visited << '\n'
            << info.GetMaxDepth() << '\n'
            << std::setprecision(3) << std::fixed << executionTime << '\n';
        fileInfoExtra.close();
    }
    
}
