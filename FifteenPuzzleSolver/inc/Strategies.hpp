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
    OperationPath bfs(State& start_state, ops::operators* order, InfoBundle& info) const;
    OperationPath dfs(State& start_state, ops::operators* order, InfoBundle& info) const;
    OperationPath astr(State& start_state, ops::heuristics heur, InfoBundle& info) const;
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
    s - stan pocz�tkowy
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
OperationPath Strategies::bfs(State& start_state, ops::operators* order, InfoBundle& info) const {
    info.visited++;

    std::queue<State> open_states;                        /// Q - queue
    std::unordered_map<Board, OperationPath, BoardHash> processed_states; /// U - set
    State* cur_state;   // for iteration
    open_states.push(start_state);                     /// Q.enqueue(s)

    while (!open_states.empty()) {                         /// while !Q.isempty():
        cur_state = &open_states.front();                 /// 	v = Q.dequeue()
        ops::operators* op = order;
        info.processed++;
        for (int i = 0; i < 4; i++, op++) {                /// for n in neighbours(v):
            State* neighbour = BoardHandler::NewMoved(*cur_state, *op); // uses new, must be deleted
            if (neighbour == nullptr)  // illegal move or trivial(for example RL or UD)
                continue;
            if (same(solvedTable, neighbour->first.table.data())) { /// if n is solution:
                // solution found!
                info.visited++; //xxx not sure if right
                info.SetMaxDepth((int)neighbour->second.path.size());
                OperationPath solution = neighbour->second;
                delete neighbour;
                return solution;							/// return success
            }
            /// if !U.has(n):
            ///     Q.enqueue(n)
            ///     U.add(v)
            auto it = processed_states.insert(*neighbour);
            // it.second informs whether insertion was successful
            // processed_states.insert returns std::pair<iterator, bool>
            if (it.second) {
                open_states.push(*neighbour); // copy is created
                info.visited++;
            }
            delete neighbour;
        }
        open_states.pop();
    }
    return { ops::NotFound }; // will display size of -1
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* dfs nowa implementacja z rozbie�no�ciami co do rekurencji:
    v - stan pocz�tkowy
    G - goal
    dfs(G, s):
        if s is solution:
            return success
        S - lista stan�w zamkni�tych (stack)
        T - lista stan�w otwartych (set)
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
OperationPath Strategies::dfs(State& start_state, ops::operators* order, InfoBundle& info) const {
    info.visited++;
    if (same(start_state.first.table.data(), solvedTable))   /// if s is solution:
        return { ops::None };										/// 	return success
    std::stack<State> open_states;									/// S - stack
    std::unordered_map<Board, OperationPath, BoardHash> processed_states;/// T - set
    bool isInserted;

    open_states.push(start_state);							/// S.push(s)
    while (!open_states.empty()) {
        info.processed++;
        State cur_state(open_states.top());						/// v = S.pop()
        open_states.pop();										///
        auto it = processed_states.insert(cur_state);
        isInserted = it.second;

        if (!isInserted) {
            if (it.first->second.GetLength() > cur_state.second.GetLength()) {
                processed_states.erase(it.first); // how long does this execute?
                processed_states.insert(cur_state);
                isInserted = true;
            }
        }
        if (isInserted && cur_state.second.path.size() < DFS_MAX_DEPTH) {
            ops::operators* op = order + 3;
            for (int i = 0; i < 4; i++, op--) {    				/// for n in neighbours(v).reverse():
                State* neighbour = BoardHandler::NewMoved(cur_state, *op); // uses new, must be deleted
                if (neighbour == nullptr)  // illegal move or trivial(for example RL or UD)
                    continue;
                info.visited++;
                info.SetMaxDepth((int)neighbour->second.path.size());
                if (same(solvedTable, neighbour->first.table.data())) { /// if n is solution:
                    OperationPath solution = neighbour->second;
                    delete neighbour;
                    return solution;						/// return success
                }

                open_states.push(*neighbour); 			/// S.push(n)
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
OperationPath Strategies::astr(State& start_state, ops::heuristics heur, InfoBundle& info) const {
    info.visited++;
    if (same(start_state.first.table.data(), solvedTable))   	/// if s is solution:
        return { ops::None };											/// 	return success

    if (heur == ops::hamm)
        heuristic = &Heuristics::Hamming;
    else if (heur == ops::manh)
        heuristic = &Heuristics::Manhattan;

    std::priority_queue<AstarState, std::vector<AstarState>, AstarCompare> open_states;	/// P - priority queue
    std::unordered_map<Board, OperationPath, BoardHash> processed_states; 	/// T - set
    ops::operators order[] = { ops::L, ops::R, ops::U, ops::D };
    const AstarState* cur_state;
    ops::operators* op;

    open_states.emplace(start_state, 0); /// P.insert(s, 0)

    while (!open_states.empty()) {
        info.processed++;
        cur_state = &open_states.top(); /// v = P.pull()
        State state_mut(cur_state->state);
        open_states.pop();

        if (same(solvedTable, state_mut.first.table.data())) { 	/// if n is solution:
            return state_mut.second;									/// 	return success
        }
        auto it = processed_states.insert(state_mut);
        if (!it.second) { 	/// if T.has(v):
                            /// 	continue
        } else {
            op = order;
            for (int i = 0; i < 4; i++, op++) {                	/// for n in neighbours(v):
                State* neighbour = BoardHandler::NewMoved(state_mut, *op); // uses new, must be deleted
                if (neighbour == nullptr)  // illegal move or trivial(for example RL or UD)
                    continue;
                info.SetMaxDepth((int)neighbour->second.path.size());
                uint16_t f = heuristic(neighbour, solvedTable)
                    + neighbour->second.GetLength();
                open_states.emplace(*neighbour, f);				/// 	P.insert(n, f)
                info.visited++;
                delete neighbour;
            }
        }

    }
    return { ops::NotFound }; /// return failure
}
