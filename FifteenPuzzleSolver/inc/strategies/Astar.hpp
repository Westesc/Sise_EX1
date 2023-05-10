#pragma once
#include "Shared.hpp"

#include "../puzzle/astar/Compare.hpp"
#include "../puzzle/astar/State.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HOW IT WORKS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// astar(G, s):
//    if s is solution:
//        return success
//    P = priority_queue()
//    T = set()
//    P.insert(s, 0) // z priorytetem zero
//    while !P.isempty():
//        v = P.pull() // najmniejszy priorytet
//        if T.has(v):
//            continue
//        if v is solution:
//            return success
//        T.add(v)
//        for n in v.neighbours:
//            f = g(n) + h(n)
//            P.insert(n, f)
//    return failure
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OperationPath astr(
    const State& startState,
    Varieties::Heuristics Heuristics,
    InfoBundle& infoBundle
) {
    infoBundle.visited++;

    if (same(startState.first.table.data(), solvedTable, Board::length))   	            /// if s is solution:
        return { Varieties::None };											            /// 	return success
    if (Heuristics == Varieties::hamm)
        heuristic = &Heuristics::Hamming;
    else if (Heuristics == Varieties::manh)
        heuristic = &Heuristics::Manhattan;

    std::priority_queue<AstarState, std::vector<AstarState>, AstarCompare> openStates;	/// P - priority queue
    std::unordered_map<Board, OperationPath, BoardHash> processedStates; 	            /// T - set
    Varieties::Operators order[] = { Varieties::L, Varieties::R, Varieties::U, Varieties::D };

    const AstarState* currentState;
    Varieties::Operators* currentOperator;

    openStates.emplace(startState, 0);                                                  /// P.insert(s, 0)

    while (!openStates.empty()) {
        infoBundle.processed++;

        currentState = &openStates.top();                                               /// v = P.pull()
        State stateMut(currentState->state);
        openStates.pop();

        if (same(solvedTable, stateMut.first.table.data(), Board::length)) 	            /// if n is solution:
            return stateMut.second;									                    /// 	return success

        auto it = processedStates.insert(stateMut);
        if (!it.second) { 	                                                            /// if T.has(v):
                                                                                        /// 	continue
        } else {
            currentOperator = order;
            for (int i = 0; i < 4; i++, currentOperator++) {                	                /// for n in neighbours(v):
                State* neighbour = BoardHandler::NewMoved(stateMut, *currentOperator);        // uses new, must be deleted

                if (neighbour == nullptr)                                               // illegal move or trivial(for example RL or UD)
                    continue;

                infoBundle.SetMaxDepth((int)neighbour->second.path.size());

                uint16 f = heuristic(neighbour, solvedTable) // heurystyka
                    + neighbour->second.GetLength(); // g³êbokoœæ

                openStates.emplace(*neighbour, f);				                        /// 	P.insert(n, f)

                infoBundle.visited++;
                delete neighbour;
            }
        }

    }
    return { Varieties::NotFound }; /// return failure
}