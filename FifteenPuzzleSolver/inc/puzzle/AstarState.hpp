#pragma once
#include "../Framework.hpp"

#include "OperationPath.hpp"
#include "Board.hpp"

#include <utility>

struct state_astr {
	uint16_t f;
	state s;

	state_astr(uint16_t f, state s) : f(f), s(std::move(s)) {}
	state_astr(const board& b, op_path p) : f(0), s(b, p) {}
	explicit state_astr(state s) : f(0), s(std::move(s)) {}
	state_astr(state s, uint16_t f) : f(f), s(std::move(s)) {}
	state_astr(const state_astr& sa) : f(sa.f), s(sa.s) {}
};
