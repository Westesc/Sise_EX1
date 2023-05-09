#pragma once
#include "../Framework.hpp"

//#include "../includes.h"

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>

// value of hashmap
struct OperationPath {
    std::vector<ops::operators> path;

//    op_path(int16_t l);
    OperationPath(const OperationPath& old, ops::operators new_op); // create moved
    OperationPath(ops::operators op);
    OperationPath() = default;

	int16_t get_length();
    std::string string() const; // called once for program, therefore returning by value
};

std::string OperationPath::string() const {
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

int16_t OperationPath::get_length() {
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
