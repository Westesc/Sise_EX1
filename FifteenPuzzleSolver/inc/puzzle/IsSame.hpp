#include "../Framework.hpp"

using Same = bool(*)(const uint8*, const uint8*, const uint8&);
Same same = nullptr;

bool IsSameMod8(
    const uint8* solved,
    const uint8* state,
    const uint8& length
) {
    auto solvedPtr = (uint64*)solved,
        statePtr = (uint64*)state;
    uint8 steps = length >> 3;
    bool result = true;

    for (uint8 i = 0; i < steps; i++, solvedPtr++, statePtr++) {
        if ((*solvedPtr ^ *statePtr) != 0) { // 0 if same
            result = false;
        }
    }

    return result;
}

bool IsSameMod4(
    const uint8* solved,
    const uint8* state,
    const uint8& length
) {
    auto solvedPtr = (uint32*)solved,
        statePtr = (uint32*)state;
    uint8 steps = length >> 2;
    bool result = true;

    for (uint8 i = 0; i < steps; i++, solvedPtr++, statePtr++) {
        if (*solvedPtr ^ *statePtr) {
            result = false;
        }
    }

    return result;
}

bool IsSameAny(
    const uint8* solved,
    const uint8* state,
    const uint8& length
) {
    auto solvedPtr = solved,
        statePtr = state;
    bool result = true;

    for (uint8 i = 0; i < length; i++, solvedPtr++, statePtr++) {
        if (*solvedPtr ^ *statePtr) {
            result = false;
        }
    }

    return result;
}