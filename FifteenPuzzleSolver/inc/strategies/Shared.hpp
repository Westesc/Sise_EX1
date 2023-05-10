#pragma once
#include "../Framework.hpp"

#include "../puzzle/BoardHandler.hpp"
#include "../puzzle/Heuristics.hpp"
#include "../InfoBundle.hpp"

#include <unordered_map>
#include <queue>
#include <stack>

constexpr auto dfsMaxDepth = 20;
constexpr auto openStatesSize = 20;

uint8* solvedTable;
using MapIterator = std::unordered_map<Board, OperationPath, BoardHash>::const_iterator;
uint16(*heuristic)(State* st, const uint8* solved);