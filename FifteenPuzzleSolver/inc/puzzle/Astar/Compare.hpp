#pragma once
#include "../../Framework.hpp"

#include "State.hpp"

struct AstarCompare {
	bool operator()(
		const AstarState& lhs, 
		const AstarState& rhs
	) {
		return lhs.f > rhs.f;
	}
};
