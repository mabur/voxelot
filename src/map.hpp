#pragma once

#include <set>
#include <string>
#include <vector>

#include "array3.hpp"
#include "vector_space.hpp"

const auto MAP_SIZE = 64;

inline int roundDoubleToInt(double x) {
    return static_cast<int>(std::round(x));
}

// x right
// y up
// z backward
struct Map {
    Array3<int> voxels;
    double voxel_length = 1.0;

    Map();

    int blockIndexX(Vector4d position_world) const;
    int blockIndexZ(Vector4d position_world) const;
    Vector4d blockPositionCenter(int z, int y, int x) const;
};
