#pragma once

// STRING DATATYPE
#include <string>
using string = std::string;
using strings = string*;

// INTEGER DATATYPEs
using uint8 = uint8_t;
using sint8 = int8_t;
using uint16 = uint16_t;
using sint16 = int16_t;
using uint32 = uint32_t;
using sint32 = int32_t;
using uint64 = uint64_t;
using sint64 = int64_t;

using size = size_t;

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <utility>

// todo: rename this namespace
namespace ops {
	enum operators : int8_t {
		L = 0b00,
		R = 0b01,
		U = 0b10,
		D = 0b11,
		None = 0b1000, NotFound = 0b1001
	};
	enum heuristics {
		hamm,
		manh,
		error
	};
}