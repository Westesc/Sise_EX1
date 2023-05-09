#pragma once
#include "../Framework.hpp"

//
// source: https://gist.github.com/gongzhitaao/7062087#file-timer-c-03-cpp-L20
//

#include <chrono>

class Timer {
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
    typedef std::chrono::duration<double, std::ratio<1, 1000> > millisecond_;
    std::chrono::time_point<clock_> beg_;

public:
    Timer() : beg_(clock_::now()) {}
    void reset();
    double elapsed() const;
};

void Timer::reset() {
    beg_ = clock_::now();
}

double Timer::elapsed() const {
    return std::chrono::duration_cast<millisecond_>
        (clock_::now() - beg_).count();
}
