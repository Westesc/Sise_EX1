#pragma once
#include "Framework.hpp"

#include "util/Timer.hpp"
#include <chrono>

class InfoBundle {

    Timer timer;
    int maxDepth;

public:

    int visited;
    int processed;

    InfoBundle();
    double GetTime();
    int GetMaxDepth() const;
    void SetMaxDepth(int new_depth);

};

InfoBundle::InfoBundle() : timer(), processed(0), visited(0), maxDepth(1) {}

double InfoBundle::GetTime() {
    double ret = timer.elapsed();
    return ret;
}

void InfoBundle::SetMaxDepth(
    int newMaxDepth
) {
    if (newMaxDepth > maxDepth)
        maxDepth = newMaxDepth;
}

int InfoBundle::GetMaxDepth() const {
    return maxDepth;
}
