#pragma once
#include "../../Framework.hpp"

#include "../OperationPath.hpp"
#include "../Board.hpp"

#include <utility>

struct AstarState {
	uint16 f;
	State state;

	AstarState(uint16_t f, State newState) : f(f), state(std::move(newState)) {}
	AstarState(const Board& newBoard, OperationPath newOperationPath) : f(0), state(newBoard, newOperationPath) {}
	explicit AstarState(State newState) : f(0), state(std::move(newState)) {}
	AstarState(State newState, uint16_t f) : f(f), state(std::move(newState)) {}
	AstarState(const AstarState& newAstarState) : f(newAstarState.f), state(newAstarState.state) {}
};
