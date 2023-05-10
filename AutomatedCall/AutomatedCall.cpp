#include "../FifteenPuzzleSolver/inc/FifteenPuzzleSolver.hpp"

#include <filesystem>
#include <iostream>

enum Algorithm : uint8 {
    BFS = 0,
    DFS = 1,
    ASTAR = 2
};

const auto selectedAlgorithm = Algorithm::ASTAR;

void runFifteenPuzzleSolver(
    const string pathProgram,
    const string rpathOutput,
    const string fileName,
    const string strategy,
    const string pathEntry,
    const uint8& paramsCount,
    strings& params
) {
    for (int i = 0; i < paramsCount; i++, params++) {
        std::stringstream pathOutput;

        pathOutput << pathProgram << rpathOutput << fileName << "_" << strategy << "_" << *params;

        const string fileResult = pathOutput.str() + "_sol.txt";
        const string fileExtra = pathOutput.str() + "_stats.txt";

        FifteenPuzzleSolver fifteenPuzzleSolver(strategy, *params, pathEntry, fileResult, fileExtra);
        fifteenPuzzleSolver.FindSolution();
    }
}

int main(int argumentsCount, char** arguments) {

    const string nameProject        ( R"(FifteenPuzzleSolver)" );
    const string rpathInput         ( R"(data\input\)" );
    const string rpathOutputBFS     ( R"(data\output\bfs\)" );
    const string rpathOutputDFS     ( R"(data\output\dfs\)" );
    const string rpathOutputASTAR   ( R"(data\output\astar\)" );

    string orders[] { "rdul", "rdlu", "drul", "drlu", "ludr", "lurd", "uldr", "ulrd" };
    string heurs[] { "hamm", "manh" };

    const string path(arguments[0]);
    const string pathProgram = path.substr(0, path.find(nameProject));
    const string pathInput = pathProgram + rpathInput;

    // // TESTING
    // getAverageTime(pathProgram);
    // return 0;

    InfoBundle info;

    for (const auto& entry : std::filesystem::directory_iterator(pathInput)) {
        const string pathEntry = entry.path().string();

        auto fnIdx = pathEntry.find(rpathInput);
        fnIdx += rpathInput.length();

        string fileName = pathEntry.substr(fnIdx);
        fileName = fileName.substr(0, fileName.find('.'));

        switch (selectedAlgorithm) {
            default:
            case BFS: {
                const string strategy = "bfs";
                const string rpathOutput = rpathOutputBFS;
                const uint8 paramsCount = 8;
                string* params = orders;
                runFifteenPuzzleSolver(pathProgram, rpathOutput, fileName, strategy, pathEntry, paramsCount, params);
            } break;
            case DFS: {
                const string strategy = "dfs";
                const string rpathOutput = rpathOutputDFS;
                const uint8 paramsCount = 8;
                string* params = orders;
                runFifteenPuzzleSolver(pathProgram, rpathOutput, fileName, strategy, pathEntry, paramsCount, params);
            } break;
            case ASTAR: {
                const string strategy = "astr";
                const string rpathOutput = rpathOutputASTAR;
                const uint8 paramsCount = 2;
                string* params = heurs;
                runFifteenPuzzleSolver(pathProgram, rpathOutput, fileName, strategy, pathEntry, paramsCount, params);
            } break;
        }
    }

    std::cout << "Elapsed Time (Execution Time): " << info.GetTime() << " ms" << std::endl;

    return 0;
}