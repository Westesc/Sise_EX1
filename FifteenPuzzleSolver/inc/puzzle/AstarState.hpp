#pragma once
#include "../Framework.hpp"

#include "OperationPath.hpp"
#include "Board.hpp"

#include <utility>

struct AstarState {
	uint16_t f;
	state s;

	AstarState(uint16_t f, state s) : f(f), s(std::move(s)) {}
	AstarState(const Board& b, OperationPath p) : f(0), s(b, p) {}
	explicit AstarState(state s) : f(0), s(std::move(s)) {}
	AstarState(state s, uint16_t f) : f(f), s(std::move(s)) {}
	AstarState(const AstarState& sa) : f(sa.f), s(sa.s) {}
};
