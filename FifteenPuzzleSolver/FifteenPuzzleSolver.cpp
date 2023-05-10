#include "inc/Framework.hpp"

#include "inc/FifteenPuzzleSolver.hpp"
#include <iostream>

int main(int argumentsCount, char** arguments) {

    const string errorParams (
        "Incorrect number of parameters!\n" 
        "Every call should have exactly 5 arguments.\n"
    );

    const string errorParamsExtra ("Incorrect number of args");
    const string pathFileOutput("../../outputMain.txt");
    const int requiredArgumentsCount = 6;

    if (argumentsCount != requiredArgumentsCount) {

        // ERROR CONSOLE
        std::cout << errorParams << std::endl;

        // ERROR INSIDE DEST FILE
        std::ofstream fileOutput(pathFileOutput);
        fileOutput << errorParamsExtra;

        return 1;
    }

    std::cout << "all good" << std::endl;

    FifteenPuzzleSolver fifteenPuzzleSolver(arguments);
    fifteenPuzzleSolver.FindSolution();

    return 0;
}