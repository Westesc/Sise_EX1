#pragma once
#include "Framework.hpp"

#include "info/InfoBundle.hpp"
#include "FileStartState.hpp"
#include "Strategies.hpp"

#include <iomanip>

class Manager {
    InfoBundle info;
    string strategy, params, 
        fileStart, fileResult, fileExtra;

    static ops::operators* GetOrder(string state);
    static ops::heuristics GetHeuristic(string state);


public:
    Manager(string strategy, string params, string s_file, string r_file, string ex_file);
    Manager(char** arguments);

    void FindSolution();
};

Manager::Manager(char** argv) : info(), strategy(argv[1]), params(argv[2]), fileStart(argv[3]), fileResult(argv[4]), fileExtra(argv[5]) {}

Manager::Manager(string strategy, string params, string s_file, string e_file, string ex_file) :
    info(), strategy(std::move(strategy)), params(std::move(params)), fileStart(std::move(s_file)),
    fileResult(std::move(e_file)), fileExtra(std::move(ex_file)) {}

ops::operators* Manager::GetOrder(string state) {
    if (state.size() == 4) {
        auto order = new ops::operators[4];
        for (int i = 0; i < 4; i++) {
            switch (state[i]) {
                case 'L':
                case 'l':
                    order[i] = ops::L;
                    break;
                case 'R':
                case 'r':
                    order[i] = ops::R;
                    break;
                case 'U':
                case 'u':
                    order[i] = ops::U;
                    break;
                case 'D':
                case 'd':
                    order[i] = ops::D;
                    break;
                default:
                    throw std::logic_error("illegal operators, must be permutation of L, R, U, D");
            }
        }
        return order;
    }
    throw std::logic_error("incorrect operators count");
}

ops::heuristics Manager::GetHeuristic(string state) {
    std::transform(state.begin(), state.end(), state.begin(), ::tolower);
    if (state == "hamm") {
        return ops::hamm;
    }
    if (state == "manh") {
        return ops::manh;
    }
    return ops::error;
}

void Manager::FindSolution() {
    FileStartState startStateHandler(fileStart);
    State start_state = startStateHandler.GetState();
    OperationPath solution;
    Strategies strats;

    if (strategy == "bfs") {
        ops::operators* order = GetOrder(params);
        solution = strats.bfs(start_state, order, info);
        delete order; // CHAOS!!!
    } else if (strategy == "dfs") {
        ops::operators* order = GetOrder(params);
        solution = strats.dfs(start_state, order, info);
        delete order;
    } else if (strategy == "astr") {
        ops::heuristics heuristic = GetHeuristic(params);
        solution = strats.astr(start_state, heuristic, info);
    }
    double execTime = info.GetTime();

    // result file
    std::ofstream solution_file(fileResult);
    solution_file << solution.GetLength() << '\n'
        << solution.GetString() << '\n';
    solution_file.close();

    // extra info file
    std::ofstream info_file(fileExtra);
    info_file << solution.GetLength() << '\n'
        << info.processed << '\n'
        << info.visited << '\n'
        << info.GetMaxDepth() << '\n'
        << std::setprecision(3) << std::fixed << execTime << '\n';
    info_file.close();
}
