#pragma once
#include "../Framework.hpp"

#include "Timer.hpp"
#include <chrono>

class info_bundle {
    timer _timer;
    int max_depth;
public:
    info_bundle();
    double get_time();
    int visited;
    int processed;
    int get_max_depth() const;
    void set_max_depth(int new_depth);
};

info_bundle::info_bundle() : _timer(), processed(0), visited(0), max_depth(1) {}

double info_bundle::get_time() {
    double ret = _timer.elapsed();
    return ret;
}

void info_bundle::set_max_depth(int new_depth) {
    if (new_depth > max_depth)
        max_depth = new_depth;
}

int info_bundle::get_max_depth() const {
    return max_depth;
}
