#pragma once
#include "../Framework.hpp"

#include "Timer.hpp"
#include <chrono>

class InfoBundle {
    Timer timer;
    int maxDepth;
public:
    InfoBundle();
    double get_time();
    int visited;
    int processed;
    int get_max_depth() const;
    void set_max_depth(int new_depth);
};

InfoBundle::InfoBundle() : timer(), processed(0), visited(0), maxDepth(1) {}

double InfoBundle::get_time() {
    double ret = timer.elapsed();
    return ret;
}

void InfoBundle::set_max_depth(int newMaxDepth) {
    if (newMaxDepth > maxDepth)
        maxDepth = newMaxDepth;
}

int InfoBundle::get_max_depth() const {
    return maxDepth;
}
