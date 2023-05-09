#pragma once
#include "Framework.hpp"

#include "puzzle/BoardHandler.hpp"
#include "puzzle/AstarCompare.hpp"
#include "puzzle/AstarState.hpp"
#include "puzzle/Heuristics.hpp"
#include "info/InfoBundle.hpp"
#include "Strategies.hpp"

#include <unordered_map>
#include <queue>
#include <stack>

#define DFS_MAX_DEPTH 20
#define OPEN_STATES_SIZE 20

struct Strategies {
    Strategies();
    ~Strategies();
    uint8* solvedTable;

	static uint16 (*heuristic)(State* st, const uint8* solved);
    OperationPath bfs(const State& start_state, ops::operators* order, InfoBundle& info) const;
    OperationPath dfs(const State& start_state, ops::operators* order, InfoBundle& info) const;
    OperationPath astr(const State& start_state, ops::heuristics heur, InfoBundle& info) const;
};

uint16 (*Strategies::heuristic)(State* st, const uint8* solved);

using MapIterator = std::unordered_map<Board, OperationPath, BoardHash>::const_iterator;

Strategies::Strategies() {
    Board::InitializeSame();
    solvedTable = BoardHandler::NewSolvedTable(); // to modify solved State pass different function
}

Strategies::~Strategies() {
    delete[](solvedTable);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* bfs wer 3:
    s - stan pocz¹tkowy
    if s is solution:
        return success
    Q = queue
    U = set
    Q.enqueue(s)
    while !Q.isempty():
        v = Q.dequeue()
        for n in neighbours(v):
            if n is solution:
                return success
            if !U.has(n):
                Q.enqueue(n)
                U.add(v)
    return failure
 */
OperationPath Strategies::bfs(
    const State& startState, 
    ops::operators* order, 
    InfoBundle& infoBundle
) const {

    infoBundle.visited++;

    std::queue<State> openStates;                                           /// Q - queue
    std::unordered_map<Board, OperationPath, BoardHash> processedStates;   /// U - set
    State* currentState;   // for iteration
    openStates.push(startState);                                            /// Q.enqueue(s)

    while (!openStates.empty()) {                                           /// while !Q.isempty():
        currentState = &openStates.front();                                    /// 	v = Q.dequeue()
        ops::operators* op = order;
        infoBundle.processed++;
        for (int i = 0; i < 4; i++, op++) {                                 /// for n in neighbours(v):
            State* neighbour = BoardHandler::NewMoved(*currentState, *op);     // uses new, must be deleted

            if (neighbour == nullptr)                                       // illegal move or trivial(for example RL or UD)
                continue;

            if (same(solvedTable, neighbour->first.table.data())) {         /// if n is solution:
                // solution found!
                infoBundle.visited++;
                infoBundle.SetMaxDepth((int)neighbour->second.path.size());
                OperationPath solution = neighbour->second;
                delete neighbour;
                return solution;							                /// return success
            }

            /// if !U.has(n):
            ///     Q.enqueue(n)
            ///     U.add(v)
            auto it = processedStates.insert(*neighbour);

            // it.second informs whether insertion was successful
            // processed_states.insert returns std::pair<iterator, bool>

            if (it.second) {
                openStates.push(*neighbour); // copy is created
                infoBundle.visited++;
            }

            delete neighbour;
        }
        openStates.pop();
    }
    return { ops::NotFound }; // will display size of -1
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* dfs nowa implementacja z rozbie¿noœciami co do rekurencji:
    v - stan pocz¹tkowy
    G - goal
    dfs(G, s):
        if s is solution:
            return success
        S - lista stanów zamkniêtych (stack)
        T - lista stanów otwartych (set)
        S.push(s)
        while !S.empty():
            v = S.pop()
            if !T.has(v):
                T.add(v)
                for n in neghbours(v).reverse():
                    if n is solution:
                        return success
                    S.push(n)
        return failure
 */
OperationPath Strategies::dfs(
    const State& startState, 
    ops::operators* order, 
    InfoBundle& infoBundle
) const {

    infoBundle.visited++;

    if (same(startState.first.table.data(), solvedTable))                   /// if s is solution:
        return { ops::None };										        /// 	return success

    std::stack<State> openStates;									        /// S - stack
    std::unordered_map<Board, OperationPath, BoardHash> processedStates;   /// T - set
    bool isInserted;

    openStates.push(startState);							                /// S.push(s)
    while (!openStates.empty()) {
        infoBundle.processed++;

        State currentState(openStates.top());						            /// v = S.pop()
        openStates.pop();										            ///
        auto it = processedStates.insert(currentState);
        isInserted = it.second;

        if (!isInserted) {
            if (it.first->second.GetLength() > currentState.second.GetLength()) {
                processedStates.erase(it.first); // how long does this execute?
                processedStates.insert(currentState);
                isInserted = true;
            }
        }

        if (isInserted && currentState.second.path.size() < DFS_MAX_DEPTH) {
            ops::operators* operators = order + 3;

            for (int i = 0; i < 4; i++, operators--) {    				            /// for n in neighbours(v).reverse():
                State* neighbour = BoardHandler::NewMoved(currentState, *operators);  // uses new, must be deleted

                if (neighbour == nullptr)                                   // illegal move or trivial(for example RL or UD)
                    continue;

                infoBundle.visited++;
                infoBundle.SetMaxDepth((int)neighbour->second.path.size());

                if (same(solvedTable, neighbour->first.table.data())) {     /// if n is solution:
                    OperationPath solution = neighbour->second;
                    delete neighbour;
                    return solution;						                /// return success
                }

                openStates.push(*neighbour); 			                    /// S.push(n)
                delete neighbour;
            }

        }
    }
    return { ops::NotFound }; /// return failure
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
astar(G, s):
    if s is solution:
        return success
    P = priority_queue()
    T = set()
    P.insert(s, 0) // z priorytetem zero
    while !P.isempty():
        v = P.pull() // najmniejszy priorytet
        if T.has(v):
            continue
        if v is solution:
            return success
        T.add(v)
        for n in v.neighbours:
            f = g(n) + h(n)
            P.insert(n, f)
    return failure
 */
OperationPath Strategies::astr(
    const State& startState, 
    ops::heuristics heuristics, 
    InfoBundle& infoBundle
) const {
    infoBundle.visited++;

    if (same(startState.first.table.data(), solvedTable))   	                        /// if s is solution:
        return { ops::None };											                /// 	return success

    if (heuristics == ops::hamm)
        heuristic = &Heuristics::Hamming;
    else if (heuristics == ops::manh)
        heuristic = &Heuristics::Manhattan;

    std::priority_queue<AstarState, std::vector<AstarState>, AstarCompare> openStates;	/// P - priority queue
    std::unordered_map<Board, OperationPath, BoardHash> processedStates; 	            /// T - set
    ops::operators order[] = { ops::L, ops::R, ops::U, ops::D };

    const AstarState* currentState;
    ops::operators* operators;

    openStates.emplace(startState, 0);                                                  /// P.insert(s, 0)

    while (!openStates.empty()) {
        infoBundle.processed++;

        currentState = &openStates.top();                                               /// v = P.pull()
        State stateMut(currentState->state);
        openStates.pop();

        if (same(solvedTable, stateMut.first.table.data())) 	                        /// if n is solution:
            return stateMut.second;									                    /// 	return success

        auto it = processedStates.insert(stateMut);
        if (!it.second) { 	                                                            /// if T.has(v):
                                                                                        /// 	continue
        } else {
            operators = order;
            for (int i = 0; i < 4; i++, operators++) {                	                /// for n in neighbours(v):
                State* neighbour = BoardHandler::NewMoved(stateMut, *operators);        // uses new, must be deleted

                if (neighbour == nullptr)                                               // illegal move or trivial(for example RL or UD)
                    continue;

                infoBundle.SetMaxDepth((int)neighbour->second.path.size());

                uint16 f = heuristic(neighbour, solvedTable)
                    + neighbour->second.GetLength();

                openStates.emplace(*neighbour, f);				                        /// 	P.insert(n, f)

                infoBundle.visited++;
                delete neighbour;
            }
        }

    }
    return { ops::NotFound }; /// return failure
}
