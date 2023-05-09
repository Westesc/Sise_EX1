#pragma once
#include "../Framework.hpp"

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

// value of hashmap
struct OperationPath {
    std::vector<ops::operators> path;

    OperationPath(const OperationPath& oldPath, ops::operators newOperators); // create moved
    OperationPath(ops::operators newOperators);
    OperationPath() = default;

	sint16 GetLength();
    string GetString() const; // called once for program, therefore returning by value
};

string OperationPath::GetString() const {
    std::stringstream ss;

    for (auto i : this->path) {
        switch (i) {
            case ops::L:
                ss << "L";
                break;
            case ops::R:
                ss << "R";
                break;
            case ops::D:
                ss << "D";
                break;
            case ops::U:
                ss << "U";
                break;
            default:
                ss << "";
                break;
        }
    }

    return ss.str();
}

OperationPath::OperationPath(const OperationPath& old, ops::operators new_op) : path(old.path.size() + 1) {
    if (!old.path.empty()) {
        std::copy(old.path.begin(), old.path.end(), path.begin());
    }
    *(path.end() - 1) = new_op;
}

OperationPath::OperationPath(ops::operators op) : path(1) {
    path.at(0) = op;
}

int16_t OperationPath::GetLength() {
    if (path.size() == 1) {
        if (path[0] == ops::NotFound) {
            return -1;
        }
        if (path[0] == ops::None) {
            return 0;
        }
    }
    return path.size();
}
