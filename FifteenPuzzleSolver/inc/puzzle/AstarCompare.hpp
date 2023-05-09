#pragma once
#include "../Framework.hpp"

#include "AstarState.hpp"

struct astr_compare {
	bool operator()(const state_astr& lhs, const state_astr& rhs) {
		return lhs.f > rhs.f;
	}
};
