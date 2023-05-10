#pragma once
#include "Shared.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HOW IT WORKS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// bfs ver 3:
//  s - stan pocz¹tkowy
//  if s is solution:
//      return success
//  Q = queue
//  U = set
//  Q.enqueue(s)
//  while !Q.isempty():
//      v = Q.dequeue()
//      for n in neighbours(v):
//          if n is solution:
//              return success
//          if !U.has(n):
//              Q.enqueue(n)
//              U.add(v)
//  return failure
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OperationPath bfs(
    const State& startState,
    Varieties::Operators* order,
    InfoBundle& infoBundle
) {

    infoBundle.visited++;

    std::queue<State> openStates;                                               /// Q - queue
    std::unordered_map<Board, OperationPath, BoardHash> processedStates;        /// U - set
    State* currentState;   // for iteration
    openStates.push(startState);                                                /// Q.enqueue(s)

    while (!openStates.empty()) {                                               /// while !Q.isempty():
        currentState = &openStates.front();                                     /// 	v = Q.dequeue()
        Varieties::Operators* presentOperator = order;
        infoBundle.processed++;
        for (int i = 0; i < 4; i++, presentOperator++) {                                     /// for n in neighbours(v):
            State* neighbour = BoardHandler::NewMoved(*currentState, *presentOperator);      // uses new, must be deleted

            if (neighbour == nullptr)                                           // illegal move or trivial(for example RL or UD)
                continue;

            if (same(solvedTable, neighbour->first.table.data(), Board::length)) {  /// if n is solution:
                // Solution Found !
                infoBundle.visited++;
                infoBundle.SetMaxDepth((int)neighbour->second.path.size());
                OperationPath solution = neighbour->second;
                delete neighbour;
                return solution;							                    /// return success
            }

            /// if !U.has(n):
            ///     Q.enqueue(n)
            ///     U.add(v)
            auto result = processedStates.insert(*neighbour); // insert - Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent key.

            // result.second informs whether insertion was successful
            // processed_states.insert returns std::pair<iterator, bool>

            if (result.second) {
                openStates.push(*neighbour); // copy is created
                infoBundle.visited++;
            }

            delete neighbour;
        }
        openStates.pop();
    }
    return { Varieties::NotFound }; // will display size of -1
}