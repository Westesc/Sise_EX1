#pragma once
#include "../Framework.hpp"

#include "OperationPath.hpp"
#include "Board.hpp"

#include <utility>

struct AstarState {
	uint16 f;
	State state;

	AstarState(uint16_t f, State state) : f(f), state(std::move(state)) {}
	AstarState(const Board& b, OperationPath p) : f(0), state(b, p) {}
	explicit AstarState(State state) : f(0), state(std::move(state)) {}
	AstarState(State state, uint16_t f) : f(f), state(std::move(state)) {}
	AstarState(const AstarState& sa) : f(sa.f), state(sa.state) {}
};
