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

    size_t widthGrid() const { return voxels.width(); }
    size_t heightGrid() const { return voxels.height(); }
    size_t depthGrid() const { return voxels.depth(); }

    Vector4d centerInWorld() const{
        return Vector4d{
            0.5 * voxel_length * widthGrid(),
            0.0, // 0.5 * voxel_length * heightGrid(),
            0.5 * voxel_length * depthGrid(),
            1.0
        };
    }

    int blockIndexX(Vector4d position_world) const;
    int blockIndexZ(Vector4d position_world) const;
    Vector4d blockPositionCenter(int z, int y, int x) const;
};
