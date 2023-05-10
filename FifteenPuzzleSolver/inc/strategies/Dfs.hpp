#pragma once
#include "Shared.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HOW IT WORKS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dfs nowa implementacja z rozbie¿noœciami co do rekurencji:
//    v - stan pocz¹tkowy
//    G - goal
//    dfs(G, s):
//        if s is solution:
//            return success
//        S - lista stanów zamkniêtych (stack)
//        T - lista stanów otwartych (set)
//        S.push(s)
//        while !S.empty():
//            v = S.pop()
//            if !T.has(v):
//                T.add(v)
//                for n in neghbours(v).reverse():
//                    if n is solution:
//                        return success
//                    S.push(n)
//        return failure
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OperationPath dfs(
    const State& startState,
    Varieties::Operators* order,
    InfoBundle& infoBundle
) {

    infoBundle.visited++;

    if (same(startState.first.table.data(), solvedTable, Board::length))                    /// if s is solution:
        return { Varieties::None };										                    /// 	return success

    std::stack<State> openStates;									                        /// S - stack
    std::unordered_map<Board, OperationPath, BoardHash> processedStates;                    /// T - set
    bool isInserted;

    openStates.push(startState);							                                /// S.push(s)
    while (!openStates.empty()) {
        infoBundle.processed++;

        State currentState(openStates.top());						                        /// v = S.pop()
        openStates.pop();										                            ///
        auto it = processedStates.insert(currentState);
        isInserted = it.second;

        if (!isInserted) {
            if (it.first->second.GetLength() > currentState.second.GetLength()) {
                processedStates.erase(it.first); // how long does this execute?
                processedStates.insert(currentState);
                isInserted = true;
            }
        }

        if (isInserted && currentState.second.path.size() < dfsMaxDepth) {
            Varieties::Operators* Operators = order + 3;

            for (int i = 0; i < 4; i++, Operators--) {    				                    /// for n in neighbours(v).reverse():
                State* neighbour = BoardHandler::NewMoved(currentState, *Operators);        // uses new, must be deleted

                if (neighbour == nullptr)                                                   // illegal move or trivial(for example RL or UD)
                    continue;

                infoBundle.visited++;
                infoBundle.SetMaxDepth((int)neighbour->second.path.size());

                if (same(solvedTable, neighbour->first.table.data(), Board::length)) {      /// if n is solution:
                    OperationPath solution = neighbour->second;
                    delete neighbour;
                    return solution;						                                /// return success
                }

                openStates.push(*neighbour); 			                                    /// S.push(n)
                delete neighbour;
            }

        }
    }
    return { Varieties::NotFound }; /// return failure
}