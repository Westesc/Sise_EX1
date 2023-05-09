#pragma once
#include "../Framework.hpp"
#include <chrono>

class Timer {

    using Clock = std::chrono::high_resolution_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;
    using Millisecond = std::chrono::duration<double, std::ratio<1, 1000>>;

    std::chrono::time_point<Clock> beginning;

public:
    Timer() : beginning(Clock::now()) {}
    void reset();
    double elapsed() const;
};

void Timer::reset() {
    beginning = Clock::now();
}

double Timer::elapsed() const {
    return std::chrono::duration_cast<Millisecond>
        (Clock::now() - beginning).count();
}
