#pragma once
#include "../Framework.hpp"

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

// value of hashmap
struct OperationPath {
    std::vector<Varieties::Operators> path;

    OperationPath(const OperationPath& oldPath, Varieties::Operators newOperators); // create moved
    OperationPath(Varieties::Operators newOperators);
    OperationPath() = default;

	sint16 GetLength();
    string GetString() const; // called once for program, therefore returning by value
};

string OperationPath::GetString() const {
    std::stringstream ss;

    for (auto i : this->path) {
        switch (i) {
            case Varieties::L:
                ss << "L";
                break;
            case Varieties::R:
                ss << "R";
                break;
            case Varieties::D:
                ss << "D";
                break;
            case Varieties::U:
                ss << "U";
                break;
            default:
                ss << "";
                break;
        }
    }

    return ss.str();
}

OperationPath::OperationPath(
    const OperationPath& oldOperationPath, 
    Varieties::Operators newOperators
) : path(oldOperationPath.path.size() + 1) {

    if (!oldOperationPath.path.empty())
        std::copy(oldOperationPath.path.begin(), oldOperationPath.path.end(), path.begin());
    *(path.end() - 1) = newOperators;
}

OperationPath::OperationPath(
    Varieties::Operators newOperators
) : path(1) {
    path.at(0) = newOperators;
}

int16_t OperationPath::GetLength() {
    if (path.size() == 1) {
        if (path[0] == Varieties::NotFound) {
            return -1;
        }
        if (path[0] == Varieties::None) {
            return 0;
        }
    }
    return path.size();
}
